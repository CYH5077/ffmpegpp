#pragma once

#include <memory>

extern "C" {
    #include "libavcodec/avcodec.h"
}

#include "AVResult.hpp"
#include "Demuxer.hpp"

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
    void setAVCodecContext(AVCodecContext* codecContext);

public: // Raw pointer
    AVCodecContext* getRawCodecContext();
    
private:
    AVCodecContext* codecContext;
};


bool createVideoDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result);
bool createAudioDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result);
bool createDecodeContext(AVCodecID codecID, AVCodecParameters* codecParameters, CodecContext* codecContext, AVResult* result);

};