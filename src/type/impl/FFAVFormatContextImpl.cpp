#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
	// FFAVFormatContextImpl ����
	FFAVFormatContextImplPtr FFAVFormatContextImpl::create() {
		return std::make_shared<FFAVFormatContextImpl>();
	}

	
	// FFAVFormatContextImpl 
	FFAVFormatContextImpl::FFAVFormatContextImpl() {
        this->formatContext = nullptr;
	}

	FFAVFormatContextImpl::~FFAVFormatContextImpl() {

	}

	AVFormatContext* FFAVFormatContextImpl::getRaw() {
		return this->formatContext;
	}

	void FFAVFormatContextImpl::setRaw(AVFormatContext* formatContext) {
		this->formatContext = formatContext;
	}
};