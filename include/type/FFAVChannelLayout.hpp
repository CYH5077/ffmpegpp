#pragma once

#include "type/ffav.hpp"

#include <memory>

namespace ff {
    class FFAVChannelLayout {
    public:
        explicit FFAVChannelLayout();
        explicit FFAVChannelLayout(FFAVChannelLayout& channelLayout);
        explicit FFAVChannelLayout(FFAVChannelLayoutImplPtr channelLayoutImpl);

        virtual ~FFAVChannelLayout();

    public:
        FFAVChannelLayoutImplPtr getImpl();

    private:
        FFAVChannelLayoutImplPtr channelLayoutImpl;
    };
};