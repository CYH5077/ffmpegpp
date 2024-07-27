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

        this->streamIndex = 0;
        this->dataType = type;
    }

    FFAVStream::~FFAVStream() {}

    FFAVStreamImplPtr FFAVStream::getImpl() {
        return this->streamImpl;
    }

    int FFAVStream::getStreamIndex() {
        return this->streamIndex;
    }

    DATA_TYPE FFAVStream::getDataType() {
        return this->dataType;
    }

    FFAVChannelLayoutPtr FFAVStream::getChannelLayout() {
        AVStream* stream = this->streamImpl->getRaw();

        FFAVChannelLayoutPtr ffavChannelLayoutPtr = FFAVChannelLayout::create();
        ffavChannelLayoutPtr->getImpl()->setRaw(stream->codecpar->ch_layout);
        return ffavChannelLayoutPtr;
    }

    void FFAVStream::setStreamIndex(int streamIndex) {
        this->streamIndex = streamIndex;
    }

    void FFAVStream::setDataType(DATA_TYPE dataType) {
        this->dataType = dataType;
    }
}
