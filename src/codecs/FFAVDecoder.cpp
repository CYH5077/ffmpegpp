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
#include "libswscale/swscale.h"
}

namespace ff {
    FFAVDecoder::FFAVDecoder(ff::FFAVCodecContextPtr videoContext, ff::FFAVCodecContextPtr audioContext) {
        this->videoContext = videoContext;
        this->audioContext = audioContext;
    }

    FFAVDecoder::~FFAVDecoder() {

    }

    AVError FFAVDecoder::decode(FFAVInputContext& inputContext, FFAVDecoderCallback callback) {
        AVError error;
        for (auto& packet : inputContext) {
            AVPacket* packetRaw = packet.getImpl()->getRaw().get();
            
            if (packetRaw->stream_index ==  inputContext.getVideoStreamIndex()) {
                error = this->decode(this->videoContext, &packet, callback);
            } else if (packetRaw->stream_index == inputContext.getAudioStreamIndex()) {
                error = this->decode(this->audioContext, &packet, callback);
            }
            av_packet_unref(packetRaw);

            if (error.getType() == AV_ERROR_TYPE::AV_EOF ||
                error.getType() == AV_ERROR_TYPE::USER_STOP) {
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
        AVPacketPtr packet = ffavPacket != nullptr ? ffavPacket->getImpl()->getRaw() : nullptr;

        // packet�� ���ڵ��ϰ� callback�� ȣ��
        int ret = avcodec_send_packet(codecContext, packet.get());
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_send_packet failed", ret, "avcodec_send_packet");
        }

        while (ret >= 0) {
            FFAVFrame frame;
            ret = avcodec_receive_frame(codecContext, frame.getImpl()->getRaw().get());
            if (ret == AVERROR(EAGAIN)) {
                return AVError(AV_ERROR_TYPE::AV_EAGAIN);
            } else if (ret == AVERROR_EOF) {
                return AVError(AV_ERROR_TYPE::AV_EOF);
            } else if (ret < 0) {
                return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_receive_frame failed", ret, "avcodec_receive_frame");
            }

            if (ffavCodecContext->isEnableCuda()) {
                FFAVFrame cudaFrame;
                AVError error = this->cudaFormatConvert(frame, &cudaFrame);
                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
                }

                cudaFrame.setType(DATA_TYPE_FROM_AV_CODEC_TYPE(codecContext->codec->type));
                error = callback(cudaFrame);
                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
                }
            } else {
                frame.setType(DATA_TYPE_FROM_AV_CODEC_TYPE(codecContext->codec->type));
                AVError error = callback(frame);
                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
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

        dst->width  = src->width;
        dst->height = src->height;
        av_frame_get_buffer(dst, 0);

        int ret = av_hwframe_transfer_data(dst, src, 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_hwframe_transfer_data failed", ret, "av_hwframe_transfer_data");
        }

        dst->pts          = src->pts;
        dst->pkt_dts      = src->pkt_dts;
        dst->best_effort_timestamp = src->best_effort_timestamp;
        dst->pkt_pos      = src->pkt_pos;
        dst->pkt_duration = src->pkt_duration;

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}