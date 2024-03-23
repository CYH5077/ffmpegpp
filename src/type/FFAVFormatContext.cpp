#include "type/FFAVFormatContext.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>


namespace ff {
    FFAVFormatContext::FFAVFormatContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVFormatContext::FFAVFormatContext(FFAVFormatContext& formatContext) {
        this->formatContextImpl = FFAVFormatContextImpl::create(formatContext.formatContextImpl);
    }

    FFAVFormatContext::FFAVFormatContext(FFAVFormatContextImplPtr formatContextImpl) {
        this->formatContextImpl = formatContextImpl;
    }

    FFAVFormatContext::~FFAVFormatContext() {

    }

    FFAVFormatContextImplPtr FFAVFormatContext::getImpl() {
        return this->formatContextImpl;
    }
};