#pragma once

#include <vector>
#include <string>

#include "ffmpeg/HWDecoderList.hpp"
#include "ffmpeg/HWEncoderList.hpp"

namespace av {
    bool isCudaVideoEncodingDecodingAvailable();
    void getCudaVideoDecoderList(HWDecoderList* hwDecoderList);
    void getCudaVideoEncoderList(HWEncoderList* hwEncoderList);
};