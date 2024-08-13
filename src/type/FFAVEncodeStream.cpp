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
        this->hwVideoCodec = HW_VIDEO_CODEC::NONE;
        this->videoCodec = VIDEO_CODEC::NONE;
        this->audioCodec = AUDIO_CODEC::NONE;

        FFAVStream::streamImpl = FFAVStreamImpl::create();
    }

    FFAVEncodeStream::~FFAVEncodeStream() {}

    AVError FFAVEncodeStream::openCodec() {
        AVError error = this->codecContext->openCodec();
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        AVStream* avEncodeStream = this->getImpl()->getRaw();
        int ret = avcodec_parameters_from_context(avEncodeStream->codecpar, this->codecContext->getImpl()->getRaw());
        if (ret < 0) {
			return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_parameters_from_context failed", ret, "avcodec_parameters_from_context");
		}

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    void FFAVEncodeStream::setBitrate(long long bitrate) {
        this->codecContext->getImpl()->getRaw()->bit_rate = bitrate;
    }

    void FFAVEncodeStream::setGOPSize(int gopSize) {
        this->codecContext->getImpl()->getRaw()->gop_size = gopSize;
    }

    void FFAVEncodeStream::setMaxBFrames(int maxBFrames) {
        this->codecContext->getImpl()->getRaw()->max_b_frames = maxBFrames;
    }

    void FFAVEncodeStream::setCodec(HW_VIDEO_CODEC codec) {
        this->hwVideoCodec = codec;

        this->videoCodec = VIDEO_CODEC::NONE;
    }

    void FFAVEncodeStream::setCodec(VIDEO_CODEC codec) {
        this->videoCodec = codec;

        this->hwVideoCodec = HW_VIDEO_CODEC::NONE;
    }

    void FFAVEncodeStream::setCodec(AUDIO_CODEC codec) {
        this->audioCodec = codec;
    }

    bool FFAVEncodeStream::isVideoStream() {
        return (this->videoCodec != VIDEO_CODEC::NONE) || (this->hwVideoCodec != HW_VIDEO_CODEC::NONE);
    }

    bool FFAVEncodeStream::isAudioStream() {
        return this->audioCodec != AUDIO_CODEC::NONE;
    }

    HW_VIDEO_CODEC FFAVEncodeStream::getHWVideoCodec() {
        return this->hwVideoCodec;
    }

    VIDEO_CODEC FFAVEncodeStream::getVideoCodec() {
        return this->videoCodec;
    }

    AUDIO_CODEC FFAVEncodeStream::getAudioCodec() {
        return this->audioCodec;
    }

    FFAVPacketListPtr FFAVEncodeStream::encode(FFAVFrameListPtr frameList, AVError* error) {
        FFAVPacketListPtr packetList = std::make_shared<FFAVPacketList>();

        AVCodecContext* avCodecContext = this->codecContext->getImpl()->getRaw();

        for (auto& frame : *frameList) {
            int ret = avcodec_send_frame(avCodecContext, frame.getImpl()->getRaw().get());
            if (ret < 0) {
                *error = AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_send_frame failed", ret, "avcodec_send_frame");
                return packetList;
            }

            while (ret >= 0) {
                FFAVPacket packet;
                ret = avcodec_receive_packet(avCodecContext, packet.getImpl()->getRaw().get());

                if (ret == AVERROR_EOF) {
                    break;
                }

                if (ret == AVERROR(EAGAIN)) {
                    break;
                }

                packet.setFrameNumber(this->codecContext->getImpl()->getRaw()->frame_num);
                packet.setStreamIndex(this->streamIndex);
                packetList->push_back(packet);
            }
        }

        return packetList;
    }

    FFAVPacketListPtr FFAVEncodeStream::flush(AVError* error) {
        FFAVFrame nullFrame;
        nullFrame.getImpl()->setNull();

        FFAVFrameListPtr frameList = std::make_shared<FFAVFrameList>();
        frameList->push_back(nullFrame);

        return this->encode(frameList, error);
    }
}
