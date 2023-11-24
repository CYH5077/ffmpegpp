#include "EncodeParameter.hpp"

namespace av {

EncodeParameter::EncodeParameter() {

}

EncodeParameter::~EncodeParameter() {

}

uint64_t EncodeParameter::getBitrate() {
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

AVPixelFormat EncodeParameter::getPixelFormat() {
    return this->pixelFormat;
}

void EncodeParameter::setBitrate(uint64_t bitrate) {
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

void EncodeParameter::setPixelFormat(AVPixelFormat pixelFormat) {
    this->pixelFormat = pixelFormat;
}
};