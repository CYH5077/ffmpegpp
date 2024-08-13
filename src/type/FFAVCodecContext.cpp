#include "type/FFAVCodecContext.hpp"

#include "error/ffav.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"

extern "C" {
#include "libavutil/opt.h"
}

namespace ff {
    FFAVCodecContextPtr FFAVCodecContext::create() {
        return std::make_shared<FFAVCodecContext>();
    }

    FFAVCodecContext::FFAVCodecContext() {
        this->codecContextImpl = FFAVCodecContextImpl::create();

        this->cudaFormat = -1;

        this->isCodecOpenFlag = false;
    }

    FFAVCodecContext::~FFAVCodecContext() {}

    FFAVCodecContextImplPtr FFAVCodecContext::getImpl() {
        return this->codecContextImpl;
    }

    void FFAVCodecContext::setCudaFormat(int cudaFormat) {
        this->cudaFormat = cudaFormat;
    }

    std::string FFAVCodecContext::getCodecName() {
        return this->codecName;
    }

    void FFAVCodecContext::setCodecName(const std::string& codecName) {
        this->codecName = codecName;
    }

    int FFAVCodecContext::getCudaFormat() {
        return this->cudaFormat;
    }

    bool FFAVCodecContext::isCudaFormat() {
        return this->cudaFormat != -1;
    }

    bool FFAVCodecContext::isCodecOpen() {
		return this->isCodecOpenFlag;
	}

    AVError FFAVCodecContext::setOpt(const std::string& key, const std::string& value) {
        AVCodecContext* codecContext = this->codecContextImpl->getRaw();
        int ret = av_opt_set(codecContext->priv_data, key.c_str(), value.c_str(), 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_opt_set failed", ret, "av_opt_set");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    bool FFAVCodecContext::findCudaFormat() {
        if (this->codecContextImpl->getRaw() == nullptr) {
            return false;
        }

        AVCodecContext* codecContext = this->codecContextImpl->getRaw();
        const AVCodecHWConfig* codecHwConfig = nullptr;
        for (int i = 0; (codecHwConfig = avcodec_get_hw_config(codecContext->codec, i)); i++) {
            if (codecHwConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                codecHwConfig->device_type == AV_HWDEVICE_TYPE_CUDA) {
                this->cudaFormat = (int)codecHwConfig->pix_fmt;
                return true;
            }
        }
        return false;
    }

    AVError FFAVCodecContext::openCodec() {
        const AVCodec* avCodec = avcodec_find_encoder_by_name(this->codecName.c_str());
        if (avCodec == nullptr) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", -1, "avcodec_find_encoder_by_name");
        }

        int ret = avcodec_open2(this->codecContextImpl->getRaw(), avCodec, nullptr);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_open2 failed", ret, "avcodec_open2");
        }

        this->isCodecOpenFlag = true;

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};
