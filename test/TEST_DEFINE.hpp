#pragma once

#include <string>

class TEST {
public:
    static const std::string MP4_FILE;
    static const std::string MKV_FILE;
    static const std::string OUTPUT_MP4_FILE;
    static const std::string OUTPUT_VIDEO_TRANSCODING_MP4_FILE;
};

const std::string TEST::MP4_FILE = "./sample.mp4";
const std::string TEST::MKV_FILE = "./sample.mkv";
const std::string TEST::OUTPUT_MP4_FILE = "./output.mp4";
const std::string TEST::OUTPUT_VIDEO_TRANSCODING_MP4_FILE = "./video_transcoding.mp4";