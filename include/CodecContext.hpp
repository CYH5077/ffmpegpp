#pragma once

#include <memory>

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "VideoEncodeParameters.hpp"
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
    int64_t    getBitrate();
    Rational   getTimeBase();
    Rational   getFrameRate();
    MEDIA_TYPE getMediaType();

    void setAVCodecContext(AVCodecContext* codecContext);
    
    bool isVaildContext();
    
public: // Raw pointer
    AVCodecContext* getRawCodecContext();

private:
    AVCodecContext* codecContext;
};
typedef std::shared_ptr<CodecContext> CodecContextPtr;


// Decode AVCodecContext
CodecContextPtr createVideoDecodeContext(Demuxer& demuxer, AVResult* result);
CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result);
CodecContextPtr createDecodeContext(int codecID, AVCodecParameters* codecParameters, AVResult* result);

// Video Encode AVCodecContext
CodecContextPtr createVideoEncodeContext(const std::string& codecName, VideoEncodeParameters& encodeParameter, AVResult* result);
CodecContextPtr createVideoEncodeContext(CODEC_ID codecID, VideoEncodeParameters& encodeParameter, AVResult* result);
CodecContextPtr createVideoEncodeContext(const AVCodec* codec, VideoEncodeParameters& encodeParameter, AVResult* result);

// Audio Encode AVCodecContext


};

