#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVStreamImplPtr FFAVStreamImpl::create() {
        return std::make_shared<FFAVStreamImpl>();
    }

    FFAVStreamImpl::FFAVStreamImpl() {
        this->stream = nullptr;
    }

    FFAVStreamImpl::~FFAVStreamImpl() {}

    AVStream* FFAVStreamImpl::getRaw() {
        return this->stream;
    }
};
