#include "type/FFAVFrame.hpp"
#include "type/impl/FFAVFrameImpl.hpp"

namespace ff {
    FFAVFrame::FFAVFrame() {
        this->frameImpl = FFAVFrameImpl::create();
    }

    FFAVFrame::FFAVFrame(FFAVFrame& frame) {
        this->frameImpl = FFAVFrameImpl::create(frame.frameImpl);
    }

    FFAVFrame::FFAVFrame(ff::FFAVFrameImplPtr frameImpl) {
        this->frameImpl = FFAVFrameImpl::create(frameImpl);
    }

    FFAVFrame::~FFAVFrame() {

    }

    FFAVFrameImplPtr FFAVFrame::getImpl() {
        return this->frameImpl;
    }
};