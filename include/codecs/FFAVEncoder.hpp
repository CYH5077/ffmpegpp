#pragma once

#include "type/ffav.hpp"

#include <functional>

namespace ff {
    typedef std::function<AVError(FFAVPacket&)> EncodeCallback;

    class FFAVEncoder {
    public:
        explicit FFAVEncoder(FFAVCodecContextPtr videoContext, FFAVCodecContextPtr audioContext);
        virtual ~FFAVEncoder();

    public:
        AVError encode(FFAVFrame& frame, EncodeCallback callback);
        AVError flush();

    private:
        AVError encodeFrame(FFAVCodecContextPtr codecContext, FFAVFrame* frame);

    private:
        EncodeCallback callback;

        FFAVCodecContextPtr videoContext;
        FFAVCodecContextPtr audioContext;
    };
};