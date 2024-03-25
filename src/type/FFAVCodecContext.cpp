#include "type/FFAVCodecContext.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"


namespace ff {
    FFAVCodecContextPtr FFAVCodecContext::create() {
        return std::make_shared<FFAVCodecContext>();
    }


    FFAVCodecContext::FFAVCodecContext() {
        this->codecContextImpl = FFAVCodecContextImpl::create();

        this->isEnableCudaFlag = false;
        this->cudaHWFormat     = -1;
    }

    FFAVCodecContext::~FFAVCodecContext() {

    }

    void FFAVCodecContext::setEnableCuda(bool flag) {
        this->isEnableCudaFlag = flag;
    }

    bool FFAVCodecContext::isEnableCuda() {
        return this->isEnableCudaFlag;
    }

    int FFAVCodecContext::getCudaHWFormat() {
        return this->cudaHWFormat;
    }

    bool FFAVCodecContext::findCUDAHWFormat() {
        if (this->codecContextImpl->getRaw() == nullptr) {
            return false;
        }

        AVCodecContext* codecContext = this->codecContextImpl->getRaw();
        const AVCodecHWConfig* codecHwConfig = nullptr;
        for (int i = 0; (codecHwConfig = avcodec_get_hw_config(codecContext->codec, i)); i++) {
            if (codecHwConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                codecHwConfig->device_type == AV_HWDEVICE_TYPE_CUDA) {
                this->cudaHWFormat = (int)codecHwConfig->pix_fmt;
                return true;
            }
        }
        return false;
    }

    FFAVCodecContextImplPtr FFAVCodecContext::getImpl() {
        return this->codecContextImpl;
    }
};