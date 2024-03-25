#include "type/impl/FFAVCodecParametersImpl.hpp"

namespace ff {
    FFAVCodecParametersImplPtr FFAVCodecParametersImpl::create() {
        return std::make_shared<FFAVCodecParametersImpl>();
    }

    FFAVCodecParametersImplPtr FFAVCodecParametersImpl::create(FFAVCodecParametersImplPtr codecParametersImpl) {
        return std::make_shared<FFAVCodecParametersImpl>(codecParametersImpl);
    }

    FFAVCodecParametersImplPtr FFAVCodecParametersImpl::create(AVCodecParameters* codecParameters) {
        return std::make_shared<FFAVCodecParametersImpl>(codecParameters);
    }



    FFAVCodecParametersImpl::FFAVCodecParametersImpl() {
        this->codecParameters = nullptr;
    }

    FFAVCodecParametersImpl::FFAVCodecParametersImpl(ff::FFAVCodecParametersImplPtr codecParametersImpl) {
        this->codecParameters = codecParametersImpl->getRaw();
    }

    FFAVCodecParametersImpl::FFAVCodecParametersImpl(AVCodecParameters* codecParameters) {
        this->codecParameters = codecParameters;
    }

    FFAVCodecParametersImpl::~FFAVCodecParametersImpl() {

    }

    AVCodecParameters* FFAVCodecParametersImpl::getRaw() {
        return this->codecParameters;
    }

    void FFAVCodecParametersImpl::setRaw(AVCodecParameters* codecParameters) {
        this->codecParameters = codecParameters;
    }
};