#include "Stream.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

Stream::Stream() {
    this->stream = nullptr;
}

Stream::Stream(AVStream* stream) {
    this->stream = stream;
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
Rational Stream::getTimebase() const {
    if (this->isValidStream() == false) {
        return Rational();
    }    
    AVRational timebase = this->stream->time_base;
    return Rational(timebase);
}

Rational Stream::getFramerate() const {
    if (this->isValidStream() == false) {
        return Rational();
    }
    AVRational framerate = this->stream->avg_frame_rate;
    return Rational(framerate);
}

AVStream* Stream::getRawStream() {
    return this->stream;
}

AVCodecParameters* Stream::getRawCodecParameters() {
    return this->stream->codecpar;
}

void Stream::setRawStream(AVStream* stream) {
    this->stream = stream;
}

};