#include "ffmpeg/VideoEncodeParameters.hpp"

extern "C" {
#include "libavformat/avformat.h"
}


namespace av {

    VideoEncodeParameters::VideoEncodeParameters() {
        this->bitrate = 0;

        this->width  = 0;
        this->height = 0;

        this->gopSize = 0;
        this->maxBFrames = 0;

        this->pixelFormat = PIXEL_FORMAT::NONE;

        this->encodeThreadCount = 1;
    }

    VideoEncodeParameters::~VideoEncodeParameters() {

    }

    unsigned long VideoEncodeParameters::getBitrate() {
        return this->bitrate;
    }

    int VideoEncodeParameters::getWidth() {
        return this->width;
    }

    int VideoEncodeParameters::getHeight() {
        return this->height;
    }

    const Rational& VideoEncodeParameters::getTimeBase() {
        return this->timebase;
    }

    int VideoEncodeParameters::getGOPSize() {
        return this->gopSize;
    }

    int VideoEncodeParameters::getMaxBFrames() {
        return this->maxBFrames;
    }

    PIXEL_FORMAT VideoEncodeParameters::getPixelFormat() {
        return this->pixelFormat;
    }

    int VideoEncodeParameters::getEncodeThreadCount() {
        return this->encodeThreadCount;
    }

    void VideoEncodeParameters::setBitrate(long long bitrate) {
        this->bitrate = bitrate;
    }

    void VideoEncodeParameters::setWidth(int width) {
        this->width = width;
    }

    void VideoEncodeParameters::setHeight(int height) {
        this->height = height;
    }

    void VideoEncodeParameters::setTimeBase(const Rational& timebase) {
        this->timebase = timebase;
    }

    void VideoEncodeParameters::setGOPSize(int gopSize) {
        this->gopSize = gopSize;
    }

    void VideoEncodeParameters::setMaxBFrames(int maxBFrames) {
        this->maxBFrames = maxBFrames;
    }

    void VideoEncodeParameters::setPixelFormat(PIXEL_FORMAT pixelFormat) {
        this->pixelFormat = pixelFormat;
    }

    void VideoEncodeParameters::setEncodeThreadCount(int threadCount) {
        this->encodeThreadCount = threadCount;
    }

};