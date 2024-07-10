#include "type/impl/FFAVCodecContextImpl.hpp"

namespace ff {
    FFAVCodecContextImpl::FFAVCodecContextImpl() {
        this->codecContext = nullptr;
    }

    FFAVCodecContextImpl::~FFAVCodecContextImpl() {
        if (this->codecContext != nullptr) {
			if (this->codecContext->hw_device_ctx != nullptr) {
				av_buffer_unref(&this->codecContext->hw_device_ctx);
			} 

            avcodec_free_context(&this->codecContext);
        }
    }

    FFAVCodecContextImplPtr FFAVCodecContextImpl::create() {
        return std::make_shared<FFAVCodecContextImpl>();
    }

    AVCodecContext* FFAVCodecContextImpl::getRaw() {
        return this->codecContext;
    }

    void FFAVCodecContextImpl::setRaw(AVCodecContext* codecContext) {
        this->codecContext = codecContext;
    }
}