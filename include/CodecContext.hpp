#pragma once

#include <memory>

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "VideoEncodeParameters.hpp"
#include "AudioEncodeParameters.hpp"
#include "Rational.hpp"

namespace av {
    class CodecContext {
    public:
        explicit CodecContext();
        explicit CodecContext(AVCodecContext* codecContext);
        virtual ~CodecContext();

    public:
        CodecContext(const CodecContext&) = delete;
        CodecContext& operator=(const CodecContext&) = delete;

    public: // getter setter
        const Rational& getTimebase();
        const Rational& getFramerate();
        int64_t    getBitrate();
        MEDIA_TYPE getMediaType();

        void setAVCodecContext(AVCodecContext* codecContext);

        bool isVaildContext();

    public: // Raw pointer
        void setRawCodeContext(AVCodecContext* codecContext);

        AVCodecContext* getRawCodecContext();

    private:
        AVCodecContext* codecContext;

        Rational timebase;
        Rational framerate;
    };
    typedef std::shared_ptr<CodecContext> CodecContextPtr;


    // Decode AVCodecContext
    CodecContextPtr createVideoDecodeContext(Demuxer& demuxer, AVResult* result);
    CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result);

    // Video Encode AVCodecContext
    CodecContextPtr createVideoEncodeContext(const std::string& codecName, VideoEncodeParameters& encodeParameter, AVResult* result);
    CodecContextPtr createVideoEncodeContext(VIDEO_CODEC_ID codecID, VideoEncodeParameters& encodeParameter, AVResult* result);

    // Audio Encode AVCodecContext
    CodecContextPtr createAudioEncodeContext(const std::string codecName, AudioEncodeParameters& encodeParameter, AVResult* result);
    CodecContextPtr createAudioEncodeContext(AUDIO_CODEC_ID codecID, AudioEncodeParameters& encodeParameters, AVResult* result);

};