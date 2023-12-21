#include "ChannelLayout.hpp"

extern "C" {
#include "libavformat/avformat.h"
}


namespace av {
    ChannelLayout::ChannelLayout() {
        this->channelLayout = nullptr;
    }

    ChannelLayout::ChannelLayout(AVChannelLayout* channelLayout) {
        this->channelLayout = channelLayout;
    }

    ChannelLayout::~ChannelLayout() {

    }

    void ChannelLayout::setRawChannelLayout(AVChannelLayout* channelLayout) {
        this->channelLayout = channelLayout;
    }

    AVChannelLayout *ChannelLayout::getRawChannelLayout() {
        return this->channelLayout;
    }
};