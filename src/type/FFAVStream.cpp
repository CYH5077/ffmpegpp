#include "type/FFAVStream.hpp"
#include "type/impl/FFAVStreamImpl.hpp"


namespace ff {
    FFAVStream::FFAVStream() {
        this->streamImpl = FFAVStreamImpl::create();
    }

    FFAVStream::FFAVStream(FFAVStream& stream) {
        this->streamImpl = FFAVStreamImpl::create(stream.streamImpl);
    }

    FFAVStream::FFAVStream(ff::FFAVStreamImplPtr streamImpl) {
        this->streamImpl = FFAVStreamImpl::create(streamImpl);
    }

    FFAVStream::~FFAVStream() {

    }

    FFAVStreamImplPtr FFAVStream::getImpl() {
        return this->streamImpl;
    }
}