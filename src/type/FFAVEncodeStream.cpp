#include "type/FFAVEncodeStream.hpp"

#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVEncodeStream::FFAVEncodeStream(DATA_TYPE type) : FFAVStream(type) {
        FFAVStream::streamImpl = FFAVStreamImpl::create();
    }

    FFAVEncodeStream::~FFAVEncodeStream() {}

    AVError FFAVEncodeStream::encode() {
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}
