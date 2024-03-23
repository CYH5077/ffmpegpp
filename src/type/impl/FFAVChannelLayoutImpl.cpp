#include "type/impl/FFAVChannelLayoutImpl.hpp"

namespace ff {
    FFAVChannelLayoutImplPtr FFAVChannelLayoutImpl::create(FFAVChannelLayoutImplPtr channelLayoutImpl) {
        return std::make_shared<FFAVChannelLayoutImpl>(channelLayoutImpl);
    }

    FFAVChannelLayoutImplPtr  FFAVChannelLayoutImpl::create(AVChannelLayout *channelLayout) {
        return std::make_shared<FFAVChannelLayoutImpl>(channelLayout);
    }


    FFAVChannelLayoutImpl::FFAVChannelLayoutImpl(FFAVChannelLayoutImplPtr channelLayoutImpl) {
        this->channelLayout = channelLayoutImpl->channelLayout;
    }

    FFAVChannelLayoutImpl::FFAVChannelLayoutImpl(AVChannelLayout *channelLayout) {
        this->channelLayout = channelLayout;
    }

    FFAVChannelLayoutImpl::~FFAVChannelLayoutImpl() {

    }

    AVChannelLayout& FFAVChannelLayoutImpl::getRaw() {
        return *this->channelLayout;
    }

    void FFAVChannelLayoutImpl::setRaw(AVChannelLayout* channelLayout) {
        this->channelLayout = channelLayout;
    }
};