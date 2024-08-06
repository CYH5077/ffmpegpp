#include "type/FFAVCodecParameters.hpp"

#include "type/impl/FFAVCodecParametersImpl.hpp"

namespace ff {
    FFAVCodecParametersPtr FFAVCodecParameters::create() {
        return std::make_shared<FFAVCodecParameters>();
    }

    FFAVCodecParameters::FFAVCodecParameters() {
        this->codecParametersImpl = FFAVCodecParametersImpl::create();
    }

    FFAVCodecParameters::~FFAVCodecParameters() {}

    FFAVCodecParametersImplPtr FFAVCodecParameters::getImpl() {
        return this->codecParametersImpl;
    }
};
