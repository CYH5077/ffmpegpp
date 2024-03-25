#include "type/FFAVInputContext.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    FFAVInputContext::FFAVInputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVInputContext::~FFAVInputContext() {
        this->close();
    }

    AVError FFAVInputContext::open(const std::string &url) {
        this->close();

        // AVFormatContext 메모리 할당.
        AVFormatContextPtr formatContext  = avformat_alloc_context();
        if (!formatContext) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_alloc_context failed", -1, "avformat_alloc_context");
        }
        this->formatContextImpl->setRaw(formatContext);

        // File open
        int ret = avformat_open_input(&formatContext, url.c_str(), nullptr, nullptr);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " open failed", ret, "avformat_open_input");
        }

        // Stream find
        ret = avformat_find_stream_info(formatContext, nullptr);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " find stream info failed", ret, "avformat_find_stream_info");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVInputContext::open(const std::string &&url) {
        return this->open(url);
    }

    void FFAVInputContext::close() {
        AVFormatContextPtr formatContext  = this->formatContextImpl->getRaw();

        // formatContext가 nullptr이 아닐경우
        if (formatContext) {
            avformat_close_input(&formatContext);
            this->formatContextImpl->setRaw(formatContext);
        }
    }

    FFAVFormatContextImplPtr FFAVInputContext::getImpl() {
        return this->formatContextImpl;
    }
};