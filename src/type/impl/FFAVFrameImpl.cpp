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

    FFAVFrameImpl::FFAVFrameImpl() {
        this->frame = std::shared_ptr<AVFrame>(newAVFrame(), freeAVFrame);
    }

    FFAVFrameImpl::~FFAVFrameImpl() {}

    AVFramePtr FFAVFrameImpl::getRaw() {
        return this->frame;
    }

    void FFAVFrameImpl::setNull() {
		this->frame = std::shared_ptr<AVFrame>(nullptr, freeAVFrame);
	}
};
