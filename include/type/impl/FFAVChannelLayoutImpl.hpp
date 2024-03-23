#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    class FFAVChannelLayoutImpl : public std::enable_shared_from_this<FFAVChannelLayoutImpl> {
    public:
        explicit FFAVChannelLayoutImpl(FFAVChannelLayoutImplPtr channelLayoutImpl);
        explicit FFAVChannelLayoutImpl(AVChannelLayout* channelLayout);

        virtual ~FFAVChannelLayoutImpl();

    public:
        static FFAVChannelLayoutImplPtr create(FFAVChannelLayoutImplPtr channelLayoutImpl);
        static FFAVChannelLayoutImplPtr create(AVChannelLayout* channelLayout);

    public:
        AVChannelLayout& getRaw();
        void             setRaw(AVChannelLayout* channelLayout);

    private:
        AVChannelLayout* channelLayout;
    };
}