#include "type/FFAVSwsContext.hpp"

#include "type/impl/FFAVSwsContextImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"

extern "C" {
#include "libswscale/swscale.h"
}

namespace ff {
	FFAVSwsContextPtr FFAVSwsContext::create(int width, int height, PICTURE_FORMAT format) {
		return std::make_shared<FFAVSwsContext>(width, height, format);
	}


	FFAVSwsContext::FFAVSwsContext(int width, int height, PICTURE_FORMAT format) {
		this->swsContextImpl = FFAVSwsContextImpl::create();

		this->width  = width;
		this->height = height;
		this->format = format;
	}

	FFAVSwsContext::~FFAVSwsContext() {
		
	}

	
	AVError FFAVSwsContext::convert(FFAVFrame& frame) {
		AVFramePtr rawSrcFrame = frame.getImpl()->getRaw();

		if (this->swsContextImpl->getRaw() == nullptr) {
			AVError error = this->newSwsContext(frame);
			if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
				return error;
			}
		}

		SwsContext* rawSwsContext = this->swsContextImpl->getRaw();
		AVFrame* rawDstFrame = av_frame_alloc();
		rawDstFrame->width  = this->width;
		rawDstFrame->height = this->height;
		rawDstFrame->format = (AVPixelFormat)PICTURE_FORMAT_TO_AV_FORMAT(this->format);
		rawDstFrame->pts     = rawSrcFrame->pts;
		rawDstFrame->pkt_dts = rawSrcFrame->pkt_dts;
		av_frame_get_buffer(rawDstFrame, 0);

		int ret = sws_scale(
			rawSwsContext, 
			rawSrcFrame->data, rawSrcFrame->linesize, 0, rawSrcFrame->height,
			rawDstFrame->data, rawDstFrame->linesize);
		if (ret < 0) {
			return AVError(AV_ERROR_TYPE::AV_ERROR, "Failed to convert frame", ret, "sws_scale");
		}

		frame.getImpl()->setRaw(rawDstFrame);

		return AVError(AV_ERROR_TYPE::SUCCESS);
	}

	FFAVSwsContextImplPtr FFAVSwsContext::getImpl() {
		return this->swsContextImpl;
	}

	int FFAVSwsContext::getWidth() {
		return this->width;
	}

	int FFAVSwsContext::getHeight() {
		return this->height;
	}

	PICTURE_FORMAT FFAVSwsContext::getFormat() {
		return this->format;
	}

	AVError FFAVSwsContext::newSwsContext(FFAVFrame& frame) {
		AVFramePtr rawFrame = frame.getImpl()->getRaw();

		SwsContext* swsContext = sws_getContext(
			rawFrame->width, rawFrame->height, (AVPixelFormat)rawFrame->format,
			this->width    , this->height    , (AVPixelFormat)PICTURE_FORMAT_TO_AV_FORMAT(this->format),
			SWS_BILINEAR, nullptr, nullptr, nullptr);
		
		if (swsContext == nullptr) {
			return AVError(AV_ERROR_TYPE::AV_ERROR, "Failed to create SwsContext is nullptr");
		}

		this->swsContextImpl->setRaw(swsContext);

		return AVError(AV_ERROR_TYPE::SUCCESS);
	}
};