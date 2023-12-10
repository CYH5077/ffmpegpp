#pragma once

#include <functional>

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Muxer.hpp"
#include "Frame.hpp"
#include "CodecContext.hpp"

namespace av {

typedef std::function<void(Packet&)> EncoderCallbackFunc;

class Encoder {
public:
    explicit Encoder(CodecContextPtr videoContext, CodecContextPtr audioContext);
    virtual~ Encoder();

public:
    Encoder(const Encoder&) = delete;
    Encoder& operator=(const Encoder&) = delete;

public: 
    bool encode(MEDIA_TYPE type, Frame& frame , EncoderCallbackFunc func, AVResult* result);
    bool flush(AVResult* result);

private:
    bool encodeFrame(AVCodecContext* avCodecContext, AVFrame* avFrame, AVResult* result);
    
private:
    EncoderCallbackFunc func;

    CodecContextPtr videoContext;
    CodecContextPtr audioContext;
};

};