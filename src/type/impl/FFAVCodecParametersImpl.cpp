#include "type/impl/FFAVCodecParametersImpl.hpp"

namespace ff {
    FFAVCodecParametersImplPtr FFAVCodecParametersImpl::create() {
        return std::make_shared<FFAVCodecParametersImpl>();
    }


    FFAVCodecParametersImpl::FFAVCodecParametersImpl() {
        this->codecParameters = nullptr;
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