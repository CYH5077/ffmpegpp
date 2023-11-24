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

void Frame::printDump() {
    std::cout << " pict_type: " << av_get_picture_type_char(this->frame->pict_type) 
              << " key-frame: " << this->frame->key_frame 
              << " pts: "       << this->frame->pts 
              << " width: "     << this->frame->width
              << " height: "    << this->frame->height
              << std::endl;
}   
AVFrame* Frame::getRawFrame() {
    return this->frame;
}

};