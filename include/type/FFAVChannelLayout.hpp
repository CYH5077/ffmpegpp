#pragma once

#include "type/impl/FFAVChannelLayoutImpl.hpp"

#include <memory>

namespace ff {
    class FFAVChannelLayout;
    typedef std::shared_ptr<FFAVChannelLayout> FFAVChannelLayoutPtr;

    class FFAVChannelLayout {
    public:
        explicit FFAVChannelLayout();
        virtual ~FFAVChannelLayout();

    public:
        static FFAVChannelLayoutPtr create();

    public:
        FFAVChannelLayoutImplPtr getImpl();

    private:
        FFAVChannelLayoutImplPtr channelLayoutImpl;
    };
};