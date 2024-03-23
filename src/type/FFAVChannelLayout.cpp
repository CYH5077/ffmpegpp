#include "type/FFAVChannelLayout.hpp"
#include "type/impl/FFAVChannelLayoutImpl.hpp"

namespace ff {
    FFAVChannelLayout::FFAVChannelLayout() {
        this->channelLayoutImpl = nullptr;
    }

    FFAVChannelLayout::FFAVChannelLayout(FFAVChannelLayout& channelLayout) {
        this->channelLayoutImpl = FFAVChannelLayoutImpl::create(channelLayout.channelLayoutImpl);
    }

    FFAVChannelLayout::FFAVChannelLayout(ff::FFAVChannelLayoutImplPtr channelLayoutImpl) {
        this->channelLayoutImpl = FFAVChannelLayoutImpl::create(channelLayoutImpl);
    }

    FFAVChannelLayout::~FFAVChannelLayout() {

    }

    FFAVChannelLayoutImplPtr FFAVChannelLayout::getImpl() {
        return this->channelLayoutImpl;
    }

};