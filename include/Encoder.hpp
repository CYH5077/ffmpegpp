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
    bool encode(Muxer& muxer, MEDIA_TYPE type, Frame& frame, AVResult* result);

private:
    bool encodeFrame(Muxer& muxer, CodecContextPtr codecContext, Frame& frame, AVResult* result);
    
private:
    CodecContextPtr videoContext;
    CodecContextPtr audioContext;
};

};