#include "AVType.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

int codecIDToAVCodecID(CODEC_ID codecID) {
    switch (codecID) {
    case CODEC_ID::H264: return AV_CODEC_ID_H264;
    case CODEC_ID::AAC : return AV_CODEC_ID_AAC;
    }
    return AV_CODEC_ID_NONE;
}

int mediaTypeToAVMediaType(MEDIA_TYPE type) {
    switch (type) {
    case MEDIA_TYPE::UNKNOWN:    return AVMEDIA_TYPE_UNKNOWN;
    case MEDIA_TYPE::VIDEO:      return AVMEDIA_TYPE_VIDEO;
    case MEDIA_TYPE::AUDIO:      return AVMEDIA_TYPE_AUDIO;
    case MEDIA_TYPE::DATA:       return AVMEDIA_TYPE_DATA;
    case MEDIA_TYPE::SUBTITLE:   return AVMEDIA_TYPE_SUBTITLE;
    case MEDIA_TYPE::ATTACHMENT: return AVMEDIA_TYPE_ATTACHMENT;
    case MEDIA_TYPE::NB:         return AVMEDIA_TYPE_NB;
    }
    return AVMEDIA_TYPE_UNKNOWN;
}

MEDIA_TYPE AVMediaTypeToMediaType(int type) {
    switch ((AVMediaType)type) {
    case AVMEDIA_TYPE_UNKNOWN:    return MEDIA_TYPE::UNKNOWN;
    case AVMEDIA_TYPE_VIDEO:      return MEDIA_TYPE::VIDEO;
    case AVMEDIA_TYPE_AUDIO:      return MEDIA_TYPE::AUDIO;
    case AVMEDIA_TYPE_DATA:       return MEDIA_TYPE::DATA;
    case AVMEDIA_TYPE_SUBTITLE:   return MEDIA_TYPE::SUBTITLE;
    case AVMEDIA_TYPE_ATTACHMENT: return MEDIA_TYPE::ATTACHMENT;
    case AVMEDIA_TYPE_NB:         return MEDIA_TYPE::NB;
    }

    return MEDIA_TYPE::UNKNOWN;
}

int pixelFormatToAVPixelFormat(PIXEL_FORMAT format) {
    switch (format) {
    case PIXEL_FORMAT::NONE:    return AV_PIX_FMT_NONE;
    case PIXEL_FORMAT::YUV420P: return AV_PIX_FMT_YUV420P;
    }

    return AV_PIX_FMT_NONE;
}

};