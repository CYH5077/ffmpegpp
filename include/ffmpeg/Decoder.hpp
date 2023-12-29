#pragma once

#include <memory>
#include <functional>

#include "ffmpeg/AVResult.hpp"
#include "ffmpeg/Demuxer.hpp"
#include "ffmpeg/Packet.hpp"
#include "ffmpeg/Frame.hpp"
#include "ffmpeg/CodecContext.hpp"

namespace av {
    typedef std::function<void(Packet&, Frame&, AVResult*)> DecoderCallbackFunc;

    class Decoder {
    public:
        explicit Decoder(CodecContextPtr videoContext, CodecContextPtr audioContext);
        virtual ~Decoder();

    public: // 이동 생성자 제거
        Decoder(const Decoder&) = delete;
        Decoder& operator=(const Decoder&) = delete;

    public:
        bool decode(Demuxer& demuxer, DecoderCallbackFunc func, AVResult* result);
        bool decode(Packet& packet, DecoderCallbackFunc func, AVResult* result);
        bool flush(AVResult* result);

        void play();
        void pause();
        void stop();

    private:
        bool decodePacket(AVCodecContext* avCodecContext, AVPacket* avPacket, DecoderCallbackFunc func, AVResult* result);

        bool callDecoderCallbackFunc(Packet& encodePacket, Frame& decodeFrame, DecoderCallbackFunc func, AVResult* result);

    private:
        DecoderCallbackFunc func;

        CodecContextPtr videoContext;
        CodecContextPtr audioContext;

        bool isPause;
        bool isStop;

        bool isCUDADecode;
    };
};