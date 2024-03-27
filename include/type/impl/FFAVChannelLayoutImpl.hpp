#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    class FFAVChannelLayoutImpl : public std::enable_shared_from_this<FFAVChannelLayoutImpl> {
    public:
        explicit FFAVChannelLayoutImpl();
        virtual ~FFAVChannelLayoutImpl();

    public:
        static FFAVChannelLayoutImplPtr create();

    public:
        AVChannelLayout getRaw();
        void            setRaw(AVChannelLayout channelLayout);

    private:
        AVChannelLayout channelLayout;
    };
}