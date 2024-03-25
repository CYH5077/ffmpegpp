#include "type/FFAVCodecParameters.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"


namespace ff {
    FFAVCodecParameters::FFAVCodecParameters() {
        this->codecParametersImpl = FFAVCodecParametersImpl::create();
    }

    FFAVCodecParameters::~FFAVCodecParameters() {

    }

    FFAVCodecParametersImplPtr FFAVCodecParameters::getImpl() {
        return this->codecParametersImpl;
    }
};