#pragma once

#include <vector>
#include <string>

#include "ffmpeg/HWDecoderList.hpp"
#include "ffmpeg/HWEncoderList.hpp"
#include "ffmpeg/AVResult.hpp"
#include "ffmpeg/Frame.hpp"

namespace av {
    bool isCudaVideoEncodingDecodingAvailable();
    void getCudaVideoDecoderList(HWDecoderList* hwDecoderList);
    void getCudaVideoEncoderList(HWEncoderList* hwEncoderList);

    bool convertFrameSizeWithToRGB(Frame& targetFrame, Frame* convertFrame, int width, int height, AVResult* result);

    long long getLocalTimeMicroseconds();
    long long getPTSTimeToMicroseconds(long long pts, Rational& timebase);
};