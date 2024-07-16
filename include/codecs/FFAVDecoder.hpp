#pragma once

#include "type/ffav.hpp"

#include "error/AVError.hpp"

#include <functional>

namespace ff {
    typedef std::function<AVError(FFAVFrame&)> FFAVDecoderCallback;

    class FFAVDecoder {
    public:
        explicit FFAVDecoder(FFAVCodecContextPtr videoContext, FFAVCodecContextPtr audioContext);
        virtual ~FFAVDecoder();


    public: // decode
        AVError decode(FFAVInputContext& inputContext, FFAVDecoderCallback callback);

    private:
        AVError decode(FFAVCodecContextPtr ffavCodecContext, FFAVPacket* ffavPacket, FFAVDecoderCallback callback);
        void flush(FFAVDecoderCallback callback);

        AVError cudaFormatConvert(FFAVFrame& srcFrame, FFAVFrame* dstFrame);

    private:
        FFAVCodecContextPtr videoContext;
        FFAVCodecContextPtr audioContext;
    };
};