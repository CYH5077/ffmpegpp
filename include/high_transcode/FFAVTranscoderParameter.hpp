#pragma once

#include "codecs/ffav.hpp"
#include "error/ffav.hpp"
#include "type/ffav.hpp"
#include "utils/ffav.hpp"

namespace ff {
    class FFAVTranscoderParameter {
    public:
        explicit FFAVTranscoderParameter(FFAVInputContext& inputContext);
        virtual ~FFAVTranscoderParameter() = default;

    public:
        AVError createDecodeContext(bool isCuda = false);
        AVError createEncodeCodecContext(VIDEO_CODEC videoCodec, AUDIO_CODEC audioCodec);
        AVError createEncodeCodecContext(HW_VIDEO_CODEC videoCodec, AUDIO_CODEC audioCodec);

    public:
        FFAVInputContext& getInputContext();

        FFAVVideoEncodeParametersPtr getVideoEncodeParameters();
        FFAVAudioEncodeParametersPtr getAudioEncodeParameters();

        FFAVCodecContextPtr getVideoDecodeContext();
        FFAVCodecContextPtr getAudioDecodeContext();

        FFAVCodecContextPtr getVideoEncodeContext();
        FFAVCodecContextPtr getAudioEncodeContext();

    private:
        FFAVInputContext& inputContext;

        FFAVCodecContextPtr videoEncodeContext;
        FFAVCodecContextPtr audioEncodeContext;

        FFAVCodecContextPtr videoDecodeContext;
        FFAVCodecContextPtr audioDecodeContext;

        FFAVVideoEncodeParametersPtr videoEncodeParameters;
        FFAVAudioEncodeParametersPtr audioEncodeParameters;
    };
};  // namespace ff
