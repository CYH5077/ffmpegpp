#include "common/FFAVPictureFormat.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    int PICTURE_FORMAT_TO_AV_FORMAT(PICTURE_FORMAT format) {
        switch (format) {
            case PICTURE_FORMAT::YUV420P:
                return (int)AV_PIX_FMT_YUV420P;
            case PICTURE_FORMAT::RGB:
                return (int)AV_PIX_FMT_RGB24;
            default:
                return (int)AV_PIX_FMT_NONE;
        }
    }
}
