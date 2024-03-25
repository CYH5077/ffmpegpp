#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVStreamImplPtr  FFAVStreamImpl::create() {
        return std::make_shared<FFAVStreamImpl>();
    }

    FFAVStreamImplPtr FFAVStreamImpl::create(FFAVStreamImplPtr streamImpl) {
        return std::make_shared<FFAVStreamImpl>(streamImpl);
    }

    FFAVStreamImplPtr FFAVStreamImpl::create(AVStream *stream) {
        return std::make_shared<FFAVStreamImpl>(stream);
    }


    FFAVStreamImpl::FFAVStreamImpl() {
        this->stream = nullptr;
    }

    FFAVStreamImpl::FFAVStreamImpl(ff::FFAVStreamImplPtr streamImpl) {
        this->stream = streamImpl->getRaw();
    }

    FFAVStreamImpl::FFAVStreamImpl(AVStream *stream) {
        this->stream = stream;
    }

    FFAVStreamImpl::~FFAVStreamImpl() {

    }

    AVStream* FFAVStreamImpl::getRaw() {
        return this->stream;
    }

    void FFAVStreamImpl::setRaw(AVStream *stream) {
        this->stream = stream;
    }
};
