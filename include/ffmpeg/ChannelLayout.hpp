#pragma once

#include "AVType.hpp"


namespace av {
    class ChannelLayout {
    public:
        explicit ChannelLayout();
        explicit ChannelLayout(AVChannelLayout* channelLayout);
        virtual ~ChannelLayout();

    public: // getter setter


    public: // Raw Pointer
        void setRawChannelLayout(AVChannelLayout* channelLayout);

        AVChannelLayout* getRawChannelLayout();

    private:
        AVChannelLayout* channelLayout;
    };
};