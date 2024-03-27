#pragma once

#include "type/ffav.hpp"
#include "error/ffav.hpp"

namespace ff::video::decode {
    FFAVCodecContextPtr createCodecContext(FFAVInputContext& inputContext , AVError* error);
    FFAVCodecContextPtr createCUDACodecContext(FFAVInputContext& inputContext , AVError* error);
};

namespace ff::audio::decode {
    FFAVCodecContextPtr createCodecContext(FFAVInputContext& inputContext, AVError* error);
};