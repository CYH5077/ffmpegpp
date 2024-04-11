#include "codecs/FFAVEncoder.hpp"

#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

namespace ff {
    FFAVEncoder::FFAVEncoder(FFAVCodecContextPtr videoContext, FFAVCodecContextPtr audioContext) {
        this->videoContext = videoContext;
        this->audioContext = audioContext;
    }

    FFAVEncoder::~FFAVEncoder() {
    }

    AVError FFAVEncoder::encode(DATA_TYPE type, FFAVFrame& frame, EncodeCallback callback) {
        this->callback = callback;

        AVError error;
        if (type == DATA_TYPE::VIDEO) {
            error = encodeFrame(this->videoContext, &frame);
        } else if (type == DATA_TYPE::AUDIO) {
            error = encodeFrame(this->audioContext, &frame);
        }
        return error;
    }

    AVError FFAVEncoder::flush() {
        AVError error;
        if (videoContext != nullptr) {
            error = this->encodeFrame(videoContext, nullptr);
        }

        if (audioContext != nullptr) {
            error = this->encodeFrame(audioContext, nullptr);
        }

        return error;
    }

    AVError FFAVEncoder::encodeFrame(FFAVCodecContextPtr codecContext, FFAVFrame* frame) {
        if (codecContext == nullptr) {
            return AVError(AV_ERROR_TYPE::SUCCESS);
        }

        AVCodecContext* codecContextRaw = codecContext->getImpl()->getRaw();
        AVFrame* frameRaw = frame == nullptr ? nullptr : frame->getImpl()->getRaw().get();


        int ret = 0;
        if (codecContextRaw->codec_type == AVMEDIA_TYPE_AUDIO && frame != nullptr) {
            int ret = av_channel_layout_copy(&frameRaw->ch_layout, &codecContextRaw->ch_layout);
            if (ret < 0) {
                return AVError(AV_ERROR_TYPE::AV_ERROR, "av_channel_layout_copy failed", ret, "av_channel_layout_copy");
            }
        }


        FFAVPacket packet;
        AVPacket* packetRaw = packet.getImpl()->getRaw().get();
        ret = avcodec_send_frame(codecContextRaw, frameRaw);
        while (ret >= 0) {
            ret = avcodec_receive_packet(codecContextRaw, packetRaw);

            if (ret == AVERROR_EOF) {
                break;
            }

            if (ret == AVERROR(EAGAIN)) {
                break;
            }

            packet.setFrameNumber(codecContextRaw->frame_num);
            packet.setType(DATA_TYPE_FROM_AV_CODEC_TYPE(codecContextRaw->codec_type));
            if (this->callback(packet) == false) {
                av_packet_unref(packetRaw);
                return AVError(AV_ERROR_TYPE::FAILED, "callback failed", -1, "callback");
            }

            av_packet_unref(packetRaw);
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};