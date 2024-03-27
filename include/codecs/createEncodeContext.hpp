#pragma once

#include "type/ffav.hpp"
#include "error/ffav.hpp"

#include "codecs/FFAVVideoEncodeParameters.hpp"
#include "codecs/FFAVAudioEncodeParameters.hpp"

#include <string>

namespace ff {
    enum class VIDEO_CODEC {
        H264,
        H265,
    };
    std::string VIDEO_CODEC_TO_STRING(VIDEO_CODEC codec);

    enum class HW_VIDEO_CODEC {
        H264,
        H265,
    };
    std::string HW_VIDEO_CODEC_TO_STRING(HW_VIDEO_CODEC codec);

    enum class AUDIO_CODEC {
        AC3,
        AAC,
    };
    std::string AUDIO_CODEC_TO_STRING(AUDIO_CODEC codec);


}
namespace ff::video::encode {
    FFAVCodecContextPtr createCodecContext(std::string&& codecName, FFAVVideoEncodeParametersPtr encodeParameters, AVError* error);
    FFAVCodecContextPtr createCodecContext(VIDEO_CODEC codec, FFAVVideoEncodeParametersPtr encodeParameters, AVError* error);
    FFAVCodecContextPtr createCUDACodecContext(HW_VIDEO_CODEC hwVideoCodec, FFAVVideoEncodeParametersPtr encodeParameters, AVError* error);

};

namespace ff::audio::encode {
    FFAVCodecContextPtr createCodecContext(std::string& codecName, FFAVAudioEncodeParametersPtr encodeParameters, AVError* error);
    FFAVCodecContextPtr createCodecContext(AUDIO_CODEC codec, FFAVAudioEncodeParametersPtr encodeParameters, AVError* error);
};