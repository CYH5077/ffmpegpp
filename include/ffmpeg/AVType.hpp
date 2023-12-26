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
struct AVChannelLayout;

namespace av {

enum class VIDEO_CODEC_ID {
    H264,
    H265,
};
int videoCodecIDToAVCodecID(VIDEO_CODEC_ID codecID);

enum class AUDIO_CODEC_ID {
    AAC,
    AC3,
};
int audioCodecIDToAVCodecID(AUDIO_CODEC_ID audioCodecId);

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
    NONE,
    YUV420P
};
int pixelFormatToAVPixelFormat(PIXEL_FORMAT format);


enum class SAMPLE_FORMAT {
    NONE,
    S16,
    S16P,
    FLTP,
};
int sampleFormatToAVSampleFormat(SAMPLE_FORMAT format);

};