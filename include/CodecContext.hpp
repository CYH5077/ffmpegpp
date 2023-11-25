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
typedef std::shared_ptr<CodecContext> CodecContextPtr;

// Decode AVCodecContext
CodecContextPtr createVideoDecodeContext(Demuxer& demuxer, AVResult* result);
CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result);
CodecContextPtr createDecodeContext(int codecID, AVCodecParameters* codecParameters, AVResult* result);

// Encode AVCodecContext
CodecContextPtr createEncodeContext(const std::string& codecName, EncodeParameter& encodeParameter, AVResult* result);
CodecContextPtr createEncodeContext(CODEC_ID codecID, EncodeParameter& encodeParameter, AVResult* result);
CodecContextPtr createEncodeContext(const AVCodec* codec, EncodeParameter& encodeParameter, AVResult* result);
};

