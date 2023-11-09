#include "FFMPEG.hpp"

#include <iostream>

extern "C" {
    #include "libavformat/avformat.h"
}

FFMPEG::FFMPEG() {

}

FFMPEG::~FFMPEG() {
    
}

void FFMPEG::printVersion() {
    const char* version = av_version_info();
    std::cout << version << std::endl;
}