#include "type/FFAVStream.hpp"
#include "type/impl/FFAVStreamImpl.hpp"


namespace ff {
    FFAVStream::FFAVStream() {
        this->streamImpl = FFAVStreamImpl::create();
    }

    FFAVStream::~FFAVStream() {

    }

    FFAVStreamImplPtr FFAVStream::getImpl() {
        return this->streamImpl;
    }
}