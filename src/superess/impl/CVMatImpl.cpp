#include "superess/impl/CVMatImpl.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

namespace ff::dnn {
    CVMatImplPtr CVMatImpl::create() {
        return std::make_shared<CVMatImpl>();
    }

    CVMatImpl::CVMatImpl() {

    }

    CVMatImpl::~CVMatImpl() {

    }

    cv::Mat CVMatImpl::getRaw() {
        return this->cvMat;
    }

    AVPixelFormat CVMatImpl::getAVFormat() {
		return this->avFrameFormat;
	}

    void CVMatImpl::setMat(cv::Mat image) {
        this->cvMat = image;
    }

    void CVMatImpl::setAVFrame(FFAVFrameImplPtr frameImpl) {
        AVFramePtr frame = frameImpl->getRaw();
        this->avFrameFormat = (AVPixelFormat)(frame->format);

        int width = frame->width;
        int height = frame->height;

        this->cvMat = cv::Mat(height, width, CV_8UC3);

        int cvLinesizes[1];
        cvLinesizes[0] = this->cvMat.step1();
        SwsContext* conversion = sws_getContext(
                width, height, (AVPixelFormat)frame->format,
                width, height, AV_PIX_FMT_BGR24,
                SWS_FAST_BILINEAR, NULL, NULL, NULL);
        sws_scale(conversion, frame->data, frame->linesize,
                  0, height, &this->cvMat.data, cvLinesizes);
        sws_freeContext(conversion);
    }

    void CVMatImpl::setAVFormat(AVPixelFormat format) {
		this->avFrameFormat = format;
	}

    FFAVFrameImplPtr CVMatImpl::toAVFrame() {
        FFAVFrameImplPtr frameImpl = FFAVFrameImpl::create();
        AVFrame* frame = frameImpl->getRaw().get();
        frame->width  = this->cvMat.cols;
        frame->height = this->cvMat.rows;
        frame->format = this->avFrameFormat;

        if (av_frame_get_buffer(frame, 0) < 0) {
            return nullptr;
		}

        SwsContext* swsContext = sws_getContext(this->cvMat.cols, this->cvMat.rows, AV_PIX_FMT_BGR24, frame->width, frame->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (!swsContext) {
            return nullptr;
        }

        uint8_t* convertFrame[1]    = { this->cvMat.data };
        int convertFrameLineSize[1] = { static_cast<int>(cvMat.step[0]) };
        sws_scale(swsContext, convertFrame, convertFrameLineSize, 0, cvMat.rows, frame->data, frame->linesize);


        return frameImpl;
    }


};