#include "type/FFAVStream.hpp"
#include "type/impl/FFAVStreamImpl.hpp"


namespace ff {
    FFAVStreamPtr FFAVStream::create() {
        return std::make_shared<FFAVStream>();
    }

    FFAVStream::FFAVStream() {
        this->streamImpl = FFAVStreamImpl::create();
    }

    FFAVStream::~FFAVStream() {

    }

    FFAVStreamImplPtr FFAVStream::getImpl() {
        return this->streamImpl;
    }
}