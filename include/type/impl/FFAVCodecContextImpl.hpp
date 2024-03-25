#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ff {
    class FFAVCodecContextImpl : public std::enable_shared_from_this<FFAVCodecContextImpl> {
    public:
        explicit FFAVCodecContextImpl();
        virtual ~FFAVCodecContextImpl();

    public:
        static FFAVCodecContextImplPtr create();

    public:
        AVCodecContext* getRaw();
        void            setRaw(AVCodecContext* codecContext);

    private:
        AVCodecContext* codecContext;
    };
};