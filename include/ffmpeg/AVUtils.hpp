#pragma once

#include <vector>
#include <string>

#include "HWDecoderList.hpp"
#include "HWEncoderList.hpp"

namespace av {
    bool isCudaVideoEncodingDecodingAvailable();
    void getCudaVideoDecoderList(HWDecoderList* hwDecoderList);
    void getCudaVideoEncoderList(HWEncoderList* hwEncoderList);
};