#include "ffmpeg/Frame.hpp"


extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>


namespace av {

    Frame::Frame() {
        this->frame = av_frame_alloc();
    }

    Frame::Frame(const Frame& frame) {
		this->frame = av_frame_clone(frame.frame);
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

    const Rational& Frame::getTimeBase() {
        return this->timebase;
    }

    unsigned char* Frame::getData(int index) {
		return this->frame->data[index];
	}

    int Frame::getWidth() {
		return this->frame->width;
	}

    int Frame::getHeight() {
        return this->frame->height;
    }

    int Frame::getLineSize(int index) {
        return this->frame->linesize[index];
    }

    long long Frame::getPTS() {
		return this->frame->pts;
	}

    void Frame::setTimeBase(Rational& timebase) {
        this->timebase = timebase;
    }

    AVFrame* Frame::getRawFrame() {
        return this->frame;
    }

    int Frame::getRawPixelFormat() {
        return this->frame->format;
    }

};