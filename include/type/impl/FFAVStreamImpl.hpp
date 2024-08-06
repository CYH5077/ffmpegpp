#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    class FFAVStreamImpl : public std::enable_shared_from_this<FFAVStreamImpl> {
    public:
        explicit FFAVStreamImpl();
        virtual ~FFAVStreamImpl();

    public:
        static FFAVStreamImplPtr create();

    public:
        AVStream* getRaw();
        
        void setRaw(AVStream* stream);

    private:
        AVStream* stream;
    };
};