#pragma once

#include "error/ffav.hpp"
#include "type/FFAVDecodeStream.hpp"
#include "type/FFAVCodecContext.hpp"

namespace ff::video::decode {
    FFAVCodecContextPtr createCodecContext(FFAVDecodeStreamPtr stream, AVError* error);
    FFAVCodecContextPtr createCUDACodecContext(FFAVDecodeStreamPtr stream, AVError* error);
};

namespace ff::audio::decode {
    FFAVCodecContextPtr createCodecContext(FFAVDecodeStreamPtr stream, AVError* error);
};
