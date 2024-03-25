#include "type/FFAVOutputContext.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    FFAVOutputContext::FFAVOutputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVOutputContext::~FFAVOutputContext() {

    }

    FFAVFormatContextImplPtr FFAVOutputContext::getImpl() {
        return this->formatContextImpl;
    }
};