#include "type/impl/FFAVFrameImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

static AVFrame* newAVFrame() {
    return av_frame_alloc();
}

static AVFrame* copyAVFrame(AVFrame* frame) {
    return av_frame_clone(frame);
}

static void freeAVFrame(AVFrame* frame) {
    av_frame_free(&frame);
}

namespace ff {
    FFAVFrameImplPtr FFAVFrameImpl::create() {
        return std::make_shared<FFAVFrameImpl>();
    }

    FFAVFrameImplPtr FFAVFrameImpl::create(FFAVFrameImplPtr frameImpl) {
        return std::make_shared<FFAVFrameImpl>(frameImpl);
    }



    FFAVFrameImpl::FFAVFrameImpl() {
        this->frame = std::shared_ptr<AVFrame>(newAVFrame(), freeAVFrame);
    }

    FFAVFrameImpl::FFAVFrameImpl(FFAVFrameImplPtr frameImpl) {
        //this->copyFrom(frameImpl);
        this->frame = frameImpl->getRaw();
    }

    FFAVFrameImpl::~FFAVFrameImpl() {

    }

    AVFramePtr FFAVFrameImpl::getRaw() {
        return this->frame;
    }

    void FFAVFrameImpl::setRaw(AVFramePtr frame) {
        this->frame = frame;
    }

    FFAVFrameImplPtr FFAVFrameImpl::copy() {
        return FFAVFrameImpl::create(this->shared_from_this());
    }

    void FFAVFrameImpl::copyFrom(FFAVFrameImplPtr frameImpl) {
        this->copyFrom(frameImpl->getRaw());
    }

    void FFAVFrameImpl::copyFrom(AVFramePtr frame) {
        AVFrame* rawFrame = copyAVFrame(frame.get());
        this->frame = std::shared_ptr<AVFrame>(rawFrame, freeAVFrame);
    }
};