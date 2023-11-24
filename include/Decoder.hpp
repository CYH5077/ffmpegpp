#pragma once

#include <functional>

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "Packet.hpp"
#include "Frame.hpp"
#include "CodecContext.hpp"

namespace av {

class Decoder {
public:
    explicit Decoder(CodecContext& videoContext, CodecContext& audioContext);
    virtual ~Decoder();

public:
    bool decode(Demuxer& demuxer, std::function<void(AVMediaType, Frame&)> func, AVResult* result);

private:
    bool decodePacket(CodecContext& codecContext, Packet& packet, Frame* frame, AVResult* result);

private:
    std::function<void(AVMediaType, Frame&)> func;

    CodecContext& videoContext;
    CodecContext& audioContext;
};

};