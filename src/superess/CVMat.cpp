#include "superess/CVMat.hpp"
#include "superess/impl/CVMatImpl.hpp"

#include "type/impl/FFAVFrameImpl.hpp"

namespace ff::dnn {
    CVMat::CVMat() {
        cvMatImpl = CVMatImpl::create();
    }

    CVMat::~CVMat() {

    }

    void CVMat::setMat(CVMat& mat) {
        cvMatImpl->setMat(mat.cvMatImpl->getRaw());
    }

    void CVMat::setFrame(FFAVFrame& frame) {
        FFAVFrameImplPtr frameImpl = frame.getImpl();
        cvMatImpl->setAVFrame(frameImpl);
    }

    void CVMat::setAVFormat(int format) {
		cvMatImpl->setAVFormat((AVPixelFormat)format);
    }

    int CVMat::getAVFormat() {
		return cvMatImpl->getAVFormat();
	}

    FFAVFrame CVMat::toFrame() {
        return FFAVFrame(cvMatImpl->toAVFrame());
    }

    CVMatImplPtr CVMat::getImpl() {
        return cvMatImpl;
    }
};