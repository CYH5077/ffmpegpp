#include "ffmpeg/Encoder.hpp"
#include "ffmpeg/Rational.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

namespace av {

    Encoder::Encoder(CodecContextPtr videoContext, CodecContextPtr audioContext) {
        this->videoContext = videoContext;
        this->audioContext = audioContext;
    }

    Encoder::~Encoder() {

    }

    bool Encoder::encode(MEDIA_TYPE type, Frame& frame, EncoderCallbackFunc func, AVResult* result) {
        this->func = func;

        switch (type) {
        case MEDIA_TYPE::VIDEO: return this->encodeFrame(this->videoContext->getRawCodecContext(), frame.getRawFrame(), result);
        case MEDIA_TYPE::AUDIO: return this->encodeFrame(this->audioContext->getRawCodecContext(), frame.getRawFrame(), result);
        default:
            return result->failed(-1, "Not support MEIDA_TYPE");
        }
    }

    bool Encoder::flush(AVResult* result) {
        if (result == nullptr) {
            return false;
        }

        if (this->videoContext != nullptr &&
            this->videoContext->isVaildContext()) { // AVCodecContext not nullptr
            if (this->encodeFrame(this->videoContext->getRawCodecContext(), nullptr, result) == false) {
                return result->isSuccess();
            }
        }

        return result->success();
    }

    bool Encoder::encodeFrame(AVCodecContext* avCodecContext, AVFrame* avFrame, AVResult* result) {
        if (avCodecContext == nullptr) {
            return true;
        }
        int ret = 0;

        if (avCodecContext->codec_type == AVMEDIA_TYPE_AUDIO && avFrame != nullptr) {
            int ret = av_channel_layout_copy(&avFrame->ch_layout, &avCodecContext->ch_layout);
            if (ret < 0) {
                return result->avFailed(ret);
            }
        }

        Packet packet;
        ret = avcodec_send_frame(avCodecContext, avFrame);
        while (ret >= 0) {
            ret = avcodec_receive_packet(avCodecContext, packet.getRawPacket());
            if (ret == AVERROR_EOF ||
                ret == AVERROR(EAGAIN)) {
                break;
            }

            packet.setMediaType(av::AVMediaTypeToMediaType(avCodecContext->codec->type));
            this->func(packet, result);
            if (result->isSuccess() == false) {
                return result->isSuccess();
            }

            packet.unref();
        }
        return result->success();
    }

};
