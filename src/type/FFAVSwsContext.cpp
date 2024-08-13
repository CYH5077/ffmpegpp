#include "type/FFAVSwsContext.hpp"

#include "type/impl/FFAVFrameImpl.hpp"
#include "type/impl/FFAVSwsContextImpl.hpp"

extern "C" {
#include "libswscale/swscale.h"
}

namespace ff {
    FFAVSwsContextPtr FFAVSwsContext::create(int width, int height, PICTURE_FORMAT format) {
        return std::make_shared<FFAVSwsContext>(width, height, format);
    }

    FFAVSwsContextPtr FFAVSwsContext::create(int width, int height, int avPictureFormat) {
        return std::make_shared<FFAVSwsContext>(width, height, avPictureFormat);
    }

    FFAVSwsContext::FFAVSwsContext(int width, int height, int avPictureFormat) {
        this->swsContextImpl = FFAVSwsContextImpl::create();

        this->width = width;
        this->height = height;
        this->avPictureFormat = avPictureFormat;
    }

    FFAVSwsContext::FFAVSwsContext(int width, int height, PICTURE_FORMAT pictureFormat) {
        this->swsContextImpl = FFAVSwsContextImpl::create();

        this->width = width;
        this->height = height;
        this->avPictureFormat = PICTURE_FORMAT_TO_AV_FORMAT(pictureFormat);
    }

    FFAVSwsContext::~FFAVSwsContext() {}

    AVError FFAVSwsContext::convert(FFAVFrame* frame) {
        AVFramePtr rawSrcFrame = frame->getImpl()->getRaw();

        if (this->swsContextImpl->getRaw() == nullptr) {
            AVError error = this->newSwsContext(*frame);
            if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                return error;
            }
        }

        SwsContext* rawSwsContext = this->swsContextImpl->getRaw();
        FFAVFrame dstFrame;
        AVFrame* dstAvFrame = dstFrame.getImpl()->getRaw().get();
        dstAvFrame->width = this->width;
        dstAvFrame->height = this->height;
        dstAvFrame->format = (AVPixelFormat)this->avPictureFormat;
        dstAvFrame->pts = rawSrcFrame->pts;
        dstAvFrame->pkt_dts = rawSrcFrame->pkt_dts;
        av_frame_get_buffer(dstAvFrame, 0);

        int ret = sws_scale(rawSwsContext,
                            rawSrcFrame->data,
                            rawSrcFrame->linesize,
                            0,
                            rawSrcFrame->height,
                            dstAvFrame->data,
                            dstAvFrame->linesize);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "Failed to convert frame", ret, "sws_scale");
        }

        *frame = dstFrame;

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

    AVError FFAVSwsContext::newSwsContext(FFAVFrame& frame) {
        AVFramePtr rawFrame = frame.getImpl()->getRaw();

        SwsContext* swsContext = sws_getContext(rawFrame->width,
                                                rawFrame->height,
                                                (AVPixelFormat)rawFrame->format,
                                                this->width,
                                                this->height,
                                                (AVPixelFormat)this->avPictureFormat,
                                                SWS_BILINEAR,
                                                nullptr,
                                                nullptr,
                                                nullptr);

        if (swsContext == nullptr) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "Failed to create SwsContext is nullptr");
        }

        this->swsContextImpl->setRaw(swsContext);

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};
