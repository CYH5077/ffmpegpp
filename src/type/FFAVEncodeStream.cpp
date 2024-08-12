#include "type/FFAVEncodeStream.hpp"

#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

namespace ff {
    FFAVEncodeStream::FFAVEncodeStream(DATA_TYPE type) : FFAVStream(type) {
        this->videoCodec = VIDEO_CODEC::NONE;
        this->audioCodec = AUDIO_CODEC::NONE;
        FFAVStream::streamImpl = FFAVStreamImpl::create();
    }

    FFAVEncodeStream::~FFAVEncodeStream() {}

    void FFAVEncodeStream::setCodec(VIDEO_CODEC codec) {
        this->videoCodec = codec;
    }

    void FFAVEncodeStream::setCodec(AUDIO_CODEC codec) {
        this->audioCodec = codec;
    }

    bool FFAVEncodeStream::isVideoStream() {
        return this->videoCodec != VIDEO_CODEC::NONE;
    }

    bool FFAVEncodeStream::isAudioStream() {
        return this->audioCodec != AUDIO_CODEC::NONE;
    }

    VIDEO_CODEC FFAVEncodeStream::getVideoCodec() {
        return this->videoCodec;
    }

    AUDIO_CODEC FFAVEncodeStream::getAudioCodec() {
        return this->audioCodec;
    }

    FFAVPacketListPtr FFAVEncodeStream::encode(FFAVFrameListPtr frameList, AVError* error) {
        FFAVPacketListPtr packetList = std::make_shared<FFAVPacketList>();

        for (auto& frame : *frameList) {
            FFAVPacket packet;
            
            int ret = avcodec_send_frame(this->codecContext->getImpl()->getRaw(), frame.getImpl()->getRaw().get());
            if (ret < 0) {
                *error = AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_send_frame failed", ret, "avcodec_send_frame");
            }

            while (ret >= 0) {
                ret = avcodec_receive_packet(this->codecContext->getImpl()->getRaw(), packet.getImpl()->getRaw().get());

                if (ret == AVERROR_EOF) {
                    break;
                }

                if (ret == AVERROR(EAGAIN)) {
                    break;
                }
                
                packetList->emplace_back(packet);
            }
        }

        return packetList;
    }
}
