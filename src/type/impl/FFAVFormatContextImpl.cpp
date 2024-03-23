#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

static AVFormatContext* newAVFormatContext() {
	return avformat_alloc_context();
}

static void freeAVFormatContext(AVFormatContext* formatContext) {
	avformat_free_context(formatContext);
}

namespace ff {
	// FFAVFormatContextImpl »ý¼º
	FFAVFormatContextImplPtr FFAVFormatContextImpl::create() {
		return std::make_shared<FFAVFormatContextImpl>();
	}

	FFAVFormatContextImplPtr FFAVFormatContextImpl::create(FFAVFormatContextImplPtr formatContextImpl) {
		return std::make_shared<FFAVFormatContextImpl>(formatContextImpl);
	}

	
	// FFAVFormatContextImpl 
	FFAVFormatContextImpl::FFAVFormatContextImpl() {
		this->formatContext = std::shared_ptr<AVFormatContext>(newAVFormatContext(), freeAVFormatContext);
	}

	FFAVFormatContextImpl::FFAVFormatContextImpl(FFAVFormatContextImplPtr formatContextImpl) {
		this->formatContext = std::shared_ptr<AVFormatContext>(newAVFormatContext(), freeAVFormatContext);
		this->setRaw(formatContextImpl->getRaw());
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