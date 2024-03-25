#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
	// FFAVFormatContextImpl »ý¼º
	FFAVFormatContextImplPtr FFAVFormatContextImpl::create() {
		return std::make_shared<FFAVFormatContextImpl>();
	}

	
	// FFAVFormatContextImpl 
	FFAVFormatContextImpl::FFAVFormatContextImpl() {
        this->formatContext = nullptr;
	}

	FFAVFormatContextImpl::~FFAVFormatContextImpl() {

	}

	AVFormatContextPtr FFAVFormatContextImpl::getRaw() {
		return this->formatContext;
	}

	void FFAVFormatContextImpl::setRaw(AVFormatContextPtr formatContext) {
		this->formatContext = formatContext;
	}
};