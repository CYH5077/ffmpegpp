#include "type/impl/FFAVChannelLayoutImpl.hpp"

namespace ff {
    FFAVChannelLayoutImplPtr FFAVChannelLayoutImpl::create() {
        return std::make_shared<FFAVChannelLayoutImpl>();
    }


    FFAVChannelLayoutImpl::FFAVChannelLayoutImpl() {
    }

    FFAVChannelLayoutImpl::~FFAVChannelLayoutImpl() {

    }

    AVChannelLayout FFAVChannelLayoutImpl::getRaw() {
        return this->channelLayout;
    }

    void FFAVChannelLayoutImpl::setRaw(AVChannelLayout channelLayout) {
        this->channelLayout = channelLayout;
    }
};