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
    }

    FFAVCodecContext::~FFAVCodecContext() {}

    FFAVCodecContextImplPtr FFAVCodecContext::getImpl() {
        return this->codecContextImpl;
    }

    AVError FFAVCodecContext::setOpt(const std::string& key, const std::string& value) {
        AVCodecContext* codecContext = this->codecContextImpl->getRaw();
        int ret = av_opt_set(codecContext->priv_data, key.c_str(), value.c_str(), 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_opt_set failed", ret, "av_opt_set");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};
