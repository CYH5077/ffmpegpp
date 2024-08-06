#pragma once

#include <string>

namespace ff {
    enum class DATA_TYPE { VIDEO, AUDIO, SUBTITLE, DATA, UNKNOWN };

    DATA_TYPE DATA_TYPE_FROM_AV_CODEC_TYPE(int codecType);

    enum class VIDEO_CODEC {
        H264,
        H265,
        NONE,
    };
    std::string VIDEO_CODEC_TO_STRING(VIDEO_CODEC codec);

    enum class HW_VIDEO_CODEC {
        H264,
        H265,
        NONE,
    };
    std::string HW_VIDEO_CODEC_TO_STRING(HW_VIDEO_CODEC codec);

    enum class AUDIO_CODEC {
        AC3,
        AAC,
        NONE,
    };
    std::string AUDIO_CODEC_TO_STRING(AUDIO_CODEC codec);
}
