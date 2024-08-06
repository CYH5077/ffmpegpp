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

            if (FFAVStream::codecContext->isCudaFormat()) {
                FFAVFrame convertFrame;
                AVError cudaConvertError = this->cudaFormatConvert(frame, &convertFrame);
                if (cudaConvertError.getType() != AV_ERROR_TYPE::SUCCESS) {
                    *error = cudaConvertError;
                    return nullptr;
                }

                frame = convertFrame;
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

    AVError FFAVDecodeStream::cudaFormatConvert(FFAVFrame& srcFrame, FFAVFrame* dstFrame) {
        AVFrame* src = srcFrame.getImpl()->getRaw().get();
        AVFrame* dst = dstFrame->getImpl()->getRaw().get();

        if (src->format != FFAVStream::codecContext->getCudaFormat()) {
            return AVError(
                AV_ERROR_TYPE::AV_ERROR, "srcFrame format is not cuda hw format", -1, "FFAVDecoder::cudaFormatConvert");
        }

        dst->width = src->width;
        dst->height = src->height;
        av_frame_get_buffer(dst, 0);

        int ret = av_hwframe_transfer_data(dst, src, 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_hwframe_transfer_data failed", ret, "av_hwframe_transfer_data");
        }

        dst->pts = src->pts;
        dst->pkt_dts = src->pkt_dts;
        dst->best_effort_timestamp = src->best_effort_timestamp;
        dst->pkt_pos = src->pkt_pos;
        dst->pkt_duration = src->pkt_duration;

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}
