#include "type/FFAVChannelLayout.hpp"
#include "type/impl/FFAVChannelLayoutImpl.hpp"

namespace ff {
    FFAVChannelLayoutPtr FFAVChannelLayout::create() {
        return std::make_shared<FFAVChannelLayout>();
    }


    FFAVChannelLayout::FFAVChannelLayout() {
        this->channelLayoutImpl = FFAVChannelLayoutImpl::create();
    }

    FFAVChannelLayout::~FFAVChannelLayout() {

    }

    FFAVChannelLayoutImplPtr FFAVChannelLayout::getImpl() {
        return this->channelLayoutImpl;
    }

};