#include "Stream.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>

namespace av {

    Stream::Stream() {
        this->stream = nullptr;
    }

    Stream::Stream(AVStream* stream) {
        this->setRawStream(stream);
    }

    Stream::~Stream() {

    }

    bool Stream::isValidStream() const {
        if (this->stream == nullptr) {
            return false;
        }
        return true;
    }

    // RVO
   const Rational& Stream::getTimebase() const {
        return this->timebase;
    }

   const Rational& Stream::getFramerate() const {
        return this->framerate;
    }

    AVStream* Stream::getRawStream() {
        return this->stream;
    }

    AVCodecParameters* Stream::getRawCodecParameters() {
        return this->stream->codecpar;
    }

    void Stream::setRawStream(AVStream* avStream) {
        if (avStream == nullptr) {
            return;
        }

        this->stream = avStream;

        this->timebase  = Rational(this->stream->time_base);
        this->framerate = Rational(this->stream->avg_frame_rate);
    }

};