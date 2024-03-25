#include "type/FFAVCodecParameters.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"


namespace ff {
    FFAVCodecParameters::FFAVCodecParameters() {
        this->codecParametersImpl = FFAVCodecParametersImpl::create();
    }

    FFAVCodecParameters::FFAVCodecParameters(FFAVCodecParameters& codecParameters) {
        this->codecParametersImpl = FFAVCodecParametersImpl::create(codecParameters.codecParametersImpl);
    }

    FFAVCodecParameters::FFAVCodecParameters(ff::FFAVCodecParametersImplPtr codecParametersImpl) {
        this->codecParametersImpl = FFAVCodecParametersImpl::create(codecParametersImpl);
    }

    FFAVCodecParameters::~FFAVCodecParameters() {

    }

    FFAVCodecParametersImplPtr FFAVCodecParameters::getImpl() {
        return this->codecParametersImpl;
    }
};