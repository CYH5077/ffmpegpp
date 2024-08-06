#include "type/FFAVDecodeStream.hpp"

#include "error/ffav.hpp"
#include "type/FFAVFrame.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

namespace ff {
    FFAVDecodeStream::FFAVDecodeStream(DATA_TYPE type) : FFAVStream(type) {
        FFAVStream::streamImpl = FFAVStreamImpl::create();
    }

    FFAVDecodeStream::~FFAVDecodeStream() {}

    FFAVFrameListPtr FFAVDecodeStream::decode(FFAVPacket& packet, AVError* error) {
        if (error == nullptr) {
            return nullptr;
        }

        FFAVFrameListPtr frameList = std::make_shared<FFAVFrameList>();

        AVCodecContext* avCodecContext = FFAVStream::codecContext->getImpl()->getRaw();
        AVPacket* avPacket = packet.getImpl()->getRaw().get();

        int ret = avcodec_send_packet(avCodecContext, avPacket);
        if (ret < 0) {
            *error = AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_send_packet failed", ret, "avcodec_send_packet");
        }

        while (ret >= 0) {
            FFAVFrame frame;
            ret = avcodec_receive_frame(avCodecContext, frame.getImpl()->getRaw().get());
            if (ret == AVERROR(EAGAIN)) {
                *error = AVError(AV_ERROR_TYPE::AV_EAGAIN);
                break;
            } else if (ret == AVERROR_EOF) {
                *error = AVError(AV_ERROR_TYPE::AV_EOF);
                break;
            } else if (ret < 0) {
                *error = AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_receive_frame failed", ret, "avcodec_receive_frame");
                break;
            }

            frameList->push_back(frame);
        }

        *error = AVError(AV_ERROR_TYPE::SUCCESS);
        return frameList;
    }

    FFAVFrameListPtr FFAVDecodeStream::decodeFlush() {
        FFAVPacket packet;
        packet.getImpl()->getRaw() = nullptr;

        AVError error;
        return this->decode(packet, &error);
    }
}
