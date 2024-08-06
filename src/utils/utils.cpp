#include "utils/utils.hpp"

#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"

extern "C" {
#include "libavutil/log.h"
}

namespace ff {
    void ffmpegLogOff() {
        av_log_set_level(AV_LOG_QUIET);
    }
};