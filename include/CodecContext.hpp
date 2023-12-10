#pragma once

#include <memory>

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "VideoEncodeParameter.hpp"
#include "Rational.hpp"

namespace av {

class CodecContext {
public:
    explicit CodecContext();
    explicit CodecContext(CodecContext&& other);
    explicit CodecContext(AVCodecContext* codecContext);
    virtual ~CodecContext();

public:
    CodecContext(const CodecContext&) = delete;
    CodecContext& operator=(const CodecContext&) = delete;

public: // getter setter
    Rational getTimeBase();
    Rational getFrameRate();

    bool isVaildContext();
    
    void setAVCodecContext(AVCodecContext* codecContext);

    
public: // Raw pointer
    AVCodecContext* getRawCodecContext();

public: // operator
    CodecContext& operator=(CodecContext&& other);

private:
    AVCodecContext* codecContext;
};
typedef std::shared_ptr<CodecContext> CodecContextPtr;

// Decode AVCodecContext
CodecContextPtr createVideoDecodeContext(Demuxer& demuxer, AVResult* result);
CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result);
CodecContextPtr createDecodeContext(int codecID, AVCodecParameters* codecParameters, AVResult* result);

// Video Encode AVCodecContext
CodecContextPtr createVideoEncodeContext(const std::string& codecName, VideoEncodeParameter& encodeParameter, AVResult* result);
CodecContextPtr createVideoEncodeContext(CODEC_ID codecID, VideoEncodeParameter& encodeParameter, AVResult* result);
CodecContextPtr createVideoEncodeContext(const AVCodec* codec, VideoEncodeParameter& encodeParameter, AVResult* result);

// Audio Encode AVCodecContext


};

