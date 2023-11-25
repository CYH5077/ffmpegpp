#pragma once

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Muxer.hpp"
#include "Frame.hpp"
#include "CodecContext.hpp"

namespace av {

class Encoder {
public:
    explicit Encoder(CodecContextPtr videoContext, CodecContextPtr audioContext);
    virtual~ Encoder();

public: 
    bool encode(MEDIA_TYPE type, Muxer& muxer, Frame& frame, AVResult* result);

private:
    bool encodeFrame(CodecContextPtr codecContext, Muxer& muxer, Frame& frame, AVResult* result);
private:
    CodecContextPtr videoContext;
    CodecContextPtr audioContext;

    int frameIndex;
};

};