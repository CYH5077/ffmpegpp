#include "type/FFAVStream.hpp"
#include "type/FFAVChannelLayout.hpp"

#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVStreamPtr FFAVStream::create(DATA_TYPE type) {
        return std::make_shared<FFAVStream>(type);
    }

    FFAVStream::FFAVStream(DATA_TYPE type) {
        this->streamImpl = FFAVStreamImpl::create();
    }

    FFAVStream::~FFAVStream() {}

    FFAVStreamImplPtr FFAVStream::getImpl() {
        return this->streamImpl;
    }
}
