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
    
    FFAVFrame::FFAVFrame(ff::FFAVFrameImplPtr frameImpl) {
        this->frameImpl = FFAVFrameImpl::create(frameImpl);
    }

    FFAVFrame::~FFAVFrame() {

    }

    FFAVFrameImplPtr FFAVFrame::getImpl() {
        return this->frameImpl;
    }

    void FFAVFrame::setType(DATA_TYPE type) {
        this->type = type;
    }

    DATA_TYPE FFAVFrame::getType() {
        return this->type;
    }

    void FFAVFrame::copyTs(ff::FFAVFrame& frame) {
        AVFramePtr dstFrame = this->frameImpl->getRaw();
        AVFramePtr srcFrame = frame.getImpl()->getRaw();

        dstFrame->pts = srcFrame->pts;
        dstFrame->pkt_dts = srcFrame->pkt_dts;
    }

    void FFAVFrame::ref(ff::FFAVFrame& frame) {
        this->frameImpl = frame.getImpl();
	}
};