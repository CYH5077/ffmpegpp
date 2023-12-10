#include "VideoEncodeParameter.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}


namespace av {

VideoEncodeParameter::VideoEncodeParameter() {

}

VideoEncodeParameter::~VideoEncodeParameter() {

}

unsigned long VideoEncodeParameter::getBitrate() {
    return this->bitrate;
}

int VideoEncodeParameter::getWidth() {
    return this->width;
}

int VideoEncodeParameter::getHeight() {
    return this->height;
}

const Rational& VideoEncodeParameter::getTimeBase() {
    return this->timebase;
}

const Rational& VideoEncodeParameter::getFrameRate() {
    return this->framerate;
}

int VideoEncodeParameter::getGOPSize() {
    return this->gopSize;
}

int VideoEncodeParameter::getMaxBFrames() {
    return this->maxBFrames;
}

PIXEL_FORMAT VideoEncodeParameter::getPixelFormat() {
    return this->pixelFormat;
}

int VideoEncodeParameter::getThreadCount() {
    return this->threadCount;
}

void VideoEncodeParameter::setBitrate(unsigned long bitrate) {
    this->bitrate = bitrate;
}

void VideoEncodeParameter::setWidth(int width) {
    this->width = width;
}

void VideoEncodeParameter::setHeight(int height) {
    this->height = height;
}

void VideoEncodeParameter::setTimeBase(const Rational&& timebase) {
    this->timebase = timebase;
}

void VideoEncodeParameter::setFrameRate(const Rational&& framerate) {
    this->framerate = framerate;
}

void VideoEncodeParameter::setGOPSize(int gopSize) {
    this->gopSize = gopSize;
}

void VideoEncodeParameter::setMaxBFrames(int maxBFrames) {
    this->maxBFrames = maxBFrames;
}

void VideoEncodeParameter::setPixelFormat(PIXEL_FORMAT pixelFormat) {
    this->pixelFormat = pixelFormat;
}

void VideoEncodeParameter::setThreadCount(int threadCount) {
    this->threadCount = threadCount;
}

};