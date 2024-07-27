#pragma once

#include "type/ffav.hpp"
#include "error/ffav.hpp"

namespace ff::video::decode {
    FFAVCodecContextPtr createCodecContext(FFAVStreamPtr stream, AVError* error);
    FFAVCodecContextPtr createCUDACodecContext(FFAVStreamPtr stream, AVError* error);
};

namespace ff::audio::decode {
    FFAVCodecContextPtr createCodecContext(FFAVStreamPtr stream, AVError* error);
};