#pragma once

#include <memory>
#include <functional>

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "Packet.hpp"
#include "Frame.hpp"
#include "CodecContext.hpp"

namespace av {

class Decoder {
public:
    explicit Decoder(CodecContextPtr videoContext, CodecContextPtr audioContext);
    virtual ~Decoder();

public:
    bool decode(Demuxer& demuxer, std::function<void(MEDIA_TYPE, Packet&, Frame&)> func, AVResult* result);
    
private:
    bool decodePacket(CodecContextPtr codecContext, Packet& packet, Frame* frame, AVResult* result);

private:
    std::function<void(MEDIA_TYPE, Packet&, Frame&)> func;

    CodecContextPtr videoContext;
    CodecContextPtr audioContext;
};

};