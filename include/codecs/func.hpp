#pragma once

#include "type/ffav.hpp"

namespace ff {
    namespace video {
        namespace decode {
            static FFAVCodecContextPtr createCodecContext(FFAVInputContext inputContext , AVError* error);
            static FFAVCodecContextPtr createCUDACodecContext(FFAVInputContext inputContext , AVError* error);
        }
    }

    namespace audio {
        namespace decode {
            static FFAVCodecContextPtr createCodecContext(FFAVInputContext inputContext, AVError* error);
        }
    }
};