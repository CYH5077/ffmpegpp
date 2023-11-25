#pragma once

#include <memory>

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "EncodeParameter.hpp"


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
    void setAVCodecContext(AVCodecContext* codecContext);

    bool isVaildContext();
    
public: // Raw pointer
    AVCodecContext* getRawCodecContext();

public: // operator
    CodecContext& operator=(CodecContext&& other);

private:
    AVCodecContext* codecContext;
};

// Decode AVCodecContext
bool createVideoDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result);
bool createAudioDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result);
bool createDecodeContext(int codecID, AVCodecParameters* codecParameters, CodecContext* codecContext, AVResult* result);

// Encode AVCodecContext
bool createEncodeContext(const std::string& codecName, EncodeParameter& encodeParameter, CodecContext* codecContext, AVResult* result);
bool createEncodeContext(CODEC_ID codecID, EncodeParameter& encodeParameter, CodecContext* codecContext, AVResult* result);
bool createEncodeContext(const AVCodec* codec, EncodeParameter& encodeParameter, CodecContext* codecContext, AVResult* result);
};