#include "type/FFAVFrame.hpp"

#include "type/impl/FFAVFrameImpl.hpp"

extern "C" {
#include "libswscale/swscale.h"
}

#include <memory>

namespace ff {
    FFAVFrame::FFAVFrame() {
        this->frameImpl = FFAVFrameImpl::create();
    }

    FFAVFrame::~FFAVFrame() {}

    FFAVFrameImplPtr FFAVFrame::getImpl() {
        return this->frameImpl;
    }
};
