#include "type/FFAVFrame.hpp"
#include "type/impl/FFAVFrameImpl.hpp"

namespace ff {
    FFAVFrame::FFAVFrame() {
        this->frameImpl = FFAVFrameImpl::create();
    }

    FFAVFrame::FFAVFrame(const FFAVFrame& frame) {
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