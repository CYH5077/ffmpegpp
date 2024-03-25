#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ff {
    class FFAVCodecParametersImpl : public std::enable_shared_from_this<FFAVCodecParametersImpl> {
    public:
        explicit FFAVCodecParametersImpl();
        explicit FFAVCodecParametersImpl(FFAVCodecParametersImplPtr codecParametersImpl);
        explicit FFAVCodecParametersImpl(AVCodecParameters* codecParameters);

        virtual ~FFAVCodecParametersImpl();

    public:
        static FFAVCodecParametersImplPtr create();
        static FFAVCodecParametersImplPtr create(FFAVCodecParametersImplPtr codecParametersImpl);
        static FFAVCodecParametersImplPtr create(AVCodecParameters* codecParameters);

    public:
        AVCodecParameters* getRaw();
        void               setRaw(AVCodecParameters* codecParameters);

    private:
        AVCodecParameters* codecParameters;
    };
};