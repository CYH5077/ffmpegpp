#include "codecs/FFAVDecoder.hpp"

#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

namespace ff {
    FFAVDecoder::FFAVDecoder(ff::FFAVCodecContextPtr videoContext, ff::FFAVCodecContextPtr audioContext) {
        this->videoContext = videoContext;
        this->audioContext = audioContext;
    }

    FFAVDecoder::~FFAVDecoder() {

    }

    AVError FFAVDecoder::decode(ff::FFAVInputContext inputContext, FFAVDecoderCallback callback) {
        AVError error;
        for (auto& iter : inputContext) {
            AVPacket* packet = iter.getImpl()->getRaw().get();

            if (packet->stream_index ==  inputContext.getVideoStreamIndex()) {
                error = this->decode(this->videoContext, &iter, callback);
            } else if (packet->stream_index == inputContext.getAudioStreamIndex()) {
                error = this->decode(this->audioContext, &iter, callback);
            }
            av_packet_unref(packet);

            if (error.getType() == AV_ERROR_TYPE::AV_EOF) {
                break;
            } else if (error.getType() == AV_ERROR_TYPE::AV_EAGAIN) {
                continue;
            } else if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                return error;
            }
        }

        this->flush(callback);

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVDecoder::decode(FFAVCodecContextPtr ffavCodecContext, FFAVPacket* ffavPacket, FFAVDecoderCallback callback) {
        if (ffavCodecContext == nullptr) {
            return AVError(AV_ERROR_TYPE::SUCCESS);
        }

        AVCodecContext* codecContext = ffavCodecContext->getImpl()->getRaw();
        AVPacket* packet = ffavPacket != nullptr ? ffavPacket->getImpl()->getRaw().get() : nullptr;

        // packet을 디코딩하고 callback을 호출
        FFAVFrame frame;
        FFAVFrame cudaFrame;
        int ret = avcodec_send_packet(codecContext, packet);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_send_packet failed", ret, "avcodec_send_packet");
        }

        while (ret >= 0) {
            ret = avcodec_receive_frame(codecContext, frame.getImpl()->getRaw().get());
            if (ret == AVERROR(EAGAIN)) {
                return AVError(AV_ERROR_TYPE::AV_EAGAIN);
            } else if (ret == AVERROR_EOF) {
                return AVError(AV_ERROR_TYPE::AV_EOF);
            } else if (ret < 0) {
                return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_receive_frame failed", ret, "avcodec_receive_frame");
            }

            if (ffavCodecContext->isEnableCuda()) {
                AVError error = this->cudaFormatConvert(frame, &cudaFrame);
                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
                }

                if (callback(*ffavPacket, cudaFrame) == false) {
                    break;
                }
            } else {
                if (callback(*ffavPacket, frame) == false) {
                    break;
                }
            }
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    void FFAVDecoder::flush(FFAVDecoderCallback callback) {
        if (this->videoContext != nullptr) {
            this->decode(this->videoContext, nullptr, callback);
        }

        if (this->audioContext != nullptr) {
            this->decode(this->audioContext, nullptr, callback);
        }
    }

    AVError FFAVDecoder::cudaFormatConvert(FFAVFrame& srcFrame, FFAVFrame* dstFrame) {
        AVFrame* src = srcFrame.getImpl()->getRaw().get();
        AVFrame* dst = dstFrame->getImpl()->getRaw().get();

        if (src->format != this->videoContext->getCudaHWFormat()) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "srcFrame format is not cuda hw format", -1, "FFAVDecoder::cudaFormatConvert");
        }

        int ret = av_hwframe_transfer_data(dst, src, 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_hwframe_transfer_data failed", ret, "av_hwframe_transfer_data");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}