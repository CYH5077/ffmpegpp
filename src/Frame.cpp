#include "Frame.hpp"


extern "C" {
    #include "libavformat/avformat.h"
}

#include <iostream>


namespace av {

Frame::Frame() {
    this->frame = av_frame_alloc();
}

Frame::Frame(AVFrame* frame) {
    this->frame = frame;
}

Frame::~Frame() {
    if (this->frame != nullptr) {
        av_frame_free(&this->frame);
    }
}

void Frame::unref() {
    if (this->frame != nullptr) {
        av_frame_unref(this->frame);
    }
}

Rational Frame::getTimeBase() {
    return this->timebase;
}

void Frame::setTimeBase(Rational& timebase) {
    this->timebase = timebase;
}

AVFrame* Frame::getRawFrame() {
    return this->frame;
}

};