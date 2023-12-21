#include "Decoder.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include <chrono>
#include <thread>

namespace av {

    Decoder::Decoder(CodecContextPtr videoContext, CodecContextPtr audioContext)
    : videoContext(videoContext)
    , audioContext(audioContext) {
        this->isPause = false;
        this->isStop  = false;
    }

    Decoder::~Decoder() {

    }

    bool Decoder::decode(Demuxer& demuxer, DecoderCallbackFunc func, AVResult* result) {
        if (result == nullptr) {
            return false;
        }

        Packet packet;
        while (demuxer.read(&packet, result) && this->isStop == false) {
            bool decodeResult;
            if (packet.getStreamIndex() == demuxer.getVideoStreamIndex()) {
                decodeResult = this->decodePacket(this->videoContext->getRawCodecContext(), packet.getRawPacket(), func, result);
            } else if (packet.getStreamIndex() == demuxer.getAudioStreamIndex()) {
                decodeResult = this->decodePacket(this->audioContext->getRawCodecContext(), packet.getRawPacket(), func, result);
            }
            packet.unref();

            if (result->isSuccess() == false) {
                return result->isSuccess();
            }

            if (decodeResult == false) {
                break;
            }
        }

        // flush AVCodecContext
        if (this->videoContext != nullptr &&
            this->videoContext->isVaildContext()) { // AVCodecContext not nullptr
            this->decodePacket(this->videoContext->getRawCodecContext(), nullptr, func, result);
        } else if (this->audioContext != nullptr &&
                   this->audioContext->isVaildContext()) { // AVCodecContext not nullptr
            this->decodePacket(this->audioContext->getRawCodecContext(), nullptr, func, result);
        }

        return result->success();
    }

    void Decoder::play() {
        this->isPause = false;
    }

    void Decoder::pause() {
        this->isPause = true;
    }

    void Decoder::stop() {
        this->isStop = true;
    }

    bool Decoder::decodePacket(AVCodecContext* avCodecContext, AVPacket* avPacket, DecoderCallbackFunc func, AVResult* result) {
        if (avCodecContext == nullptr) {
            return true;
        }

        int ret = 0;

        ret = avcodec_send_packet(avCodecContext, avPacket);
        if (ret < 0) {
            return result->avFailed(ret);
        }

        Frame frame;
        while (ret >= 0) {
            if (this->isPause == true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            ret = avcodec_receive_frame(avCodecContext, frame.getRawFrame());
            if (ret < 0) {
                if (ret == AVERROR_EOF ||
                    ret == AVERROR(EAGAIN)) {
                    return result->success();
                }
                return result->avFailed(ret);
            }

            Packet packet(avPacket, AVMediaTypeToMediaType(avCodecContext->codec->type));
            func(packet, frame, result);
            if (result->isSuccess() == false) {
                return result->isSuccess();
            }

            frame.unref();
        }
        return result->success();
    }

};