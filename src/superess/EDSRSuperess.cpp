#include "superess/EDSRSuperess.hpp"

#include "superess/impl/DnnSuperResImpl.hpp"
#include "superess/impl/CVMatImpl.hpp"

namespace ff::dnn {
    EDSRSuperess::EDSRSuperess() {
        dnnSuperResImpl = DnnSuperResImpl::create();
    }

    EDSRSuperess::~EDSRSuperess() {

    }

    AVError EDSRSuperess::openModel(const std::string &modelPath, EDSR_SCALE scale) {
        AVError error = this->dnnSuperResImpl->readModel(modelPath);

        this->dnnSuperResImpl->setModel("edsr", (int)scale);

        return error;
    }

    void EDSRSuperess::enableCuda(bool isEnable) {
        dnnSuperResImpl->enableCuda(isEnable);
    }

    void EDSRSuperess::upsample(CVMat& srcImage, CVMat* dstImage) {
        dnnSuperResImpl->upsample(srcImage.getImpl(), dstImage->getImpl());

        dstImage->setAVFormat(srcImage.getAVFormat());
    }

    void EDSRSuperess::upsample(FFAVFrame& srcFrame, FFAVFrame* dstFrame) {

        //////////////////////////////////////////////////////////////////////////////////////////////////////////// EDSR
        ff::dnn::CVMat image;
        ff::dnn::CVMat superessImage;
        image.setFrame(srcFrame);
        this->upsample(image, &superessImage);

        ff::FFAVFrame superessFrame(superessImage.toFrame());
        superessFrame.copyTs(srcFrame);

        dstFrame->ref(superessFrame);
	}
};