#include "common/FFAVType.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    DATA_TYPE DATA_TYPE_FROM_AV_CODEC_TYPE(int codecType) {
        switch (codecType) {
            case AVMEDIA_TYPE_VIDEO:
                return DATA_TYPE::VIDEO;
            case AVMEDIA_TYPE_AUDIO:
                return DATA_TYPE::AUDIO;
            case AVMEDIA_TYPE_SUBTITLE:
                return DATA_TYPE::SUBTITLE;
            case AVMEDIA_TYPE_DATA:
                return DATA_TYPE::DATA;
            default:
                return DATA_TYPE::UNKNOWN;
        }
    }

    std::string VIDEO_CODEC_TO_STRING(VIDEO_CODEC codec) {
        switch (codec) {
            case VIDEO_CODEC::H264:
                return "libx264";
            case VIDEO_CODEC::H265:
                return "libx265";
        }

        return "NONE";
    }

    std::string HW_VIDEO_CODEC_TO_STRING(HW_VIDEO_CODEC codec) {
        switch (codec) {
            case HW_VIDEO_CODEC::H264:
                return "h264_nvenc";
            case HW_VIDEO_CODEC::H265:
                return "hevc_nvenc";
        }

        return "NONE";
    }

    std::string AUDIO_CODEC_TO_STRING(AUDIO_CODEC codec) {
        switch (codec) {
            case AUDIO_CODEC::AC3:
                return "ac3";
            case AUDIO_CODEC::AAC:
                return "aac";
        }

        return "NONE";
    }
}
