#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ff {
    class FFAVCodecParametersImpl : public std::enable_shared_from_this<FFAVCodecParametersImpl> {
    public:
        explicit FFAVCodecParametersImpl();
        virtual ~FFAVCodecParametersImpl();

    public:
        static FFAVCodecParametersImplPtr create();

    public:
        AVCodecParameters* getRaw();
        void               setRaw(AVCodecParameters* codecParameters);

    private:
        AVCodecParameters* codecParameters;
    };
};