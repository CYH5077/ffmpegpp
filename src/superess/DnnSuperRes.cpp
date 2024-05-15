#include "superess/DnnSuperRes.hpp"

#include "superess/impl/DnnSuperResImpl.hpp"
#include "superess/impl/CVMatImpl.hpp"

namespace ff::dnn {
    DnnSuperRes::DnnSuperRes() {
        dnnSuperResImpl = DnnSuperResImpl::create();
    }

    DnnSuperRes::~DnnSuperRes() {

    }

    AVError DnnSuperRes::openModel(const std::string& modelPath, SUPERESS_SCALE scale) {
        AVError error = this->dnnSuperResImpl->readModel(modelPath);

        std::string modelName = SUPERESS_MODEL_TO_STRING(this->getModelType());

        this->dnnSuperResImpl->setModel(modelName, (int)scale);

        return error;
    }

    void DnnSuperRes::enableCuda(bool isEnable) {
        dnnSuperResImpl->enableCuda(isEnable);
    }

    void DnnSuperRes::upsample(CVMat& srcImage, CVMat* dstImage) {
        dnnSuperResImpl->upsample(srcImage.getImpl(), dstImage->getImpl());

        dstImage->setAVFormat(srcImage.getAVFormat());
    }

    void DnnSuperRes::upsample(FFAVFrame& srcFrame, FFAVFrame* dstFrame) {

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