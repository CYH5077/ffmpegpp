#include "EncodeParameter.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}


namespace av {

EncodeParameter::EncodeParameter() {

}

EncodeParameter::~EncodeParameter() {

}

unsigned long EncodeParameter::getBitrate() {
    return this->bitrate;
}

int EncodeParameter::getWidth() {
    return this->width;
}

int EncodeParameter::getHeight() {
    return this->height;
}

const Rational& EncodeParameter::getTimeBase() {
    return this->timebase;
}

const Rational& EncodeParameter::getFrameRate() {
    return this->framerate;
}

int EncodeParameter::getGOPSize() {
    return this->gopSize;
}

int EncodeParameter::getMaxBFrames() {
    return this->maxBFrames;
}

PIXEL_FORMAT EncodeParameter::getPixelFormat() {
    return this->pixelFormat;
}

void EncodeParameter::setBitrate(unsigned long bitrate) {
    this->bitrate = bitrate;
}

void EncodeParameter::setWidth(int width) {
    this->width = width;
}

void EncodeParameter::setHeight(int height) {
    this->height = height;
}

void EncodeParameter::setTimeBase(const Rational&& timebase) {
    this->timebase = timebase;
}

void EncodeParameter::setFrameRate(const Rational&& framerate) {
    this->framerate = framerate;
}

void EncodeParameter::setGOPSize(int gopSize) {
    this->gopSize = gopSize;
}

void EncodeParameter::setMaxBFrames(int maxBFrames) {
    this->maxBFrames = maxBFrames;
}

void EncodeParameter::setPixelFormat(PIXEL_FORMAT pixelFormat) {
    this->pixelFormat = pixelFormat;
}
};