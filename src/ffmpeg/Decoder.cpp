#include "ffmpeg/Decoder.hpp"

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

        this->isCUDADecode = this->videoContext->isCUDAContext();
    }

    Decoder::~Decoder() {

    }

    bool Decoder::decode(Demuxer& demuxer, DecoderCallbackFunc func, AVResult* result) {
        if (result == nullptr) {
            return false;
        }
        this->func = func;

        Packet packet;
        while (demuxer.read(&packet, result) && this->isStop == false) {
            bool decodeResult;
            if (packet.getStreamIndex() == demuxer.getVideoStreamIndex()) {
                decodeResult = this->decodePacket(this->videoContext->getRawCodecContext(), packet.getRawPacket(), this->func, result);
            } else if (packet.getStreamIndex() == demuxer.getAudioStreamIndex()) {
                decodeResult = this->decodePacket(this->audioContext->getRawCodecContext(), packet.getRawPacket(), this->func, result);
            }
            packet.unref();

            if (result->isSuccess() == false) {
                return result->isSuccess();
            }

            if (decodeResult == false) {
                break;
            }
        }

        if (result->isSuccess() == false &&
            result->isFileEOF() == false) {
            return result->isSuccess();
        }

        this->flush(result);

        return result->success();
    }

    bool Decoder::decode(Packet& packet, DecoderCallbackFunc func, AVResult *result) {
        if (result == nullptr) {
            return false;
        }
        this->func = func;

        switch (packet.getMediaType()) {
            case MEDIA_TYPE::VIDEO:
                return this->decodePacket(this->videoContext->getRawCodecContext(), packet.getRawPacket(), this->func, result);
            case MEDIA_TYPE::AUDIO:
                return this->decodePacket(this->audioContext->getRawCodecContext(), packet.getRawPacket(), this->func, result);
            default:
                return result->failed(-1, "MEDIA_TYPE is NONE");
        }
        return result->success();
    }

    bool Decoder::flush(AVResult* result) {
        // flush AVCodecContext
        if (this->videoContext != nullptr &&
            this->videoContext->isVaildContext()) { // AVCodecContext not nullptr
            this->decodePacket(this->videoContext->getRawCodecContext(), nullptr, func, result);
        }

        if (this->audioContext != nullptr &&
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

    void Decoder::clearFrameBuffer() {
        avcodec_flush_buffers(this->videoContext->getRawCodecContext());
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
                    return result->success(AVERROR_EOF);
                }
                return result->avFailed(ret);
            }

            AVFrame* avFrame = frame.getRawFrame();
            avFrame->pts = avFrame->best_effort_timestamp;
            Packet packet(avPacket, AVMediaTypeToMediaType(avCodecContext->codec->type));
            if (this->callDecoderCallbackFunc(packet, frame, func, result) == false) {
                return result->isSuccess();
            }

            frame.unref();
        }
        return result->success();
    }

    bool Decoder::callDecoderCallbackFunc(Packet& encodePacket, av::Frame& decodeFrame, av::DecoderCallbackFunc func, AVResult* result) {
        // CUDA 사용할 경우
        if (this->isCUDADecode              == true &&
            decodeFrame.getRawPixelFormat() == this->videoContext->getRawHWFormat()) {
            Frame swFrame;
            int ret = av_hwframe_transfer_data(swFrame.getRawFrame(), decodeFrame.getRawFrame(), 0);
            if (ret < 0) {
                return result->avFailed(ret);
            }

            func(encodePacket, swFrame, result);
            if (result->isSuccess() == false) {
                return result->isSuccess();
            }
        } else {
            func(encodePacket, decodeFrame, result);
            if (result->isSuccess() == false) {
                return result->isSuccess();
            }
        }

        return result->success();
    }

};