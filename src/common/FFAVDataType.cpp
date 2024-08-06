#include "common/FFAVDataType.hpp"

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
}
