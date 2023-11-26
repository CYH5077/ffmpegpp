    #pragma once


struct AVRational;
struct AVCodec;
struct AVCodecContext;
struct AVFormatContext;
struct AVCodecParameters;
struct AVStream;
struct AVFrame;
struct AVFormatContext;
struct AVPacket;

namespace av {

enum class CODEC_ID {
    H264,
    
    AAC,
};
int codecIDToAVCodecID(CODEC_ID codecID);


enum class MEDIA_TYPE {
    UNKNOWN = -1,
    VIDEO,
    AUDIO,
    DATA,
    SUBTITLE,
    ATTACHMENT,
    NB,
};
int mediaTypeToAVMediaType(MEDIA_TYPE type);
MEDIA_TYPE AVMediaTypeToMediaType(int type);

enum class PIXEL_FORMAT {
    YUV420P
};
int pixelFormatToAVPixelFormat(PIXEL_FORMAT format);

};