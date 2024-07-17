#include "high_transcode/FFAVTranscoderParameter.hpp"

#include "codecs/ffav.hpp"
#include "error/AVError.hpp"
#include "utils/ffav.hpp"

namespace ff {
    FFAVTranscoderParameter::FFAVTranscoderParameter(FFAVInputContext& inputContext) : inputContext(inputContext), hlsSegmentDuration(4) {
        // create parameters
        this->videoEncodeParameters = FFAVVideoEncodeParameters::create(inputContext);
        this->audioEncodeParameters = FFAVAudioEncodeParameters::create(inputContext);
    }

    AVError FFAVTranscoderParameter::createDecodeContext(bool isCuda) {
        AVError error;

        // create decode context
        if (isCuda == false) {
            this->videoDecodeContext = video::decode::createCodecContext(inputContext, &error);
        } else {
            this->videoDecodeContext = video::decode::createCUDACodecContext(inputContext, &error);
        }

        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        this->audioDecodeContext = audio::decode::createCodecContext(inputContext, &error);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVTranscoderParameter::createEncodeCodecContext(VIDEO_CODEC videoCodec, AUDIO_CODEC audioCodec) {
        AVError error;

        // create encode context
        this->videoEncodeContext =
            video::encode::createCodecContext(videoCodec, this->getVideoEncodeParameters(), &error);
        this->audioEncodeContext =
            audio::encode::createCodecContext(audioCodec, this->getAudioEncodeParameters(), &error);

        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVTranscoderParameter::createEncodeCodecContext(HW_VIDEO_CODEC videoCodec, AUDIO_CODEC audioCodec) {
        AVError error;

        // create encode context
        this->videoEncodeContext =
            video::encode::createCUDACodecContext(videoCodec, this->getVideoEncodeParameters(), &error);
        this->audioEncodeContext =
            audio::encode::createCodecContext(audioCodec, this->getAudioEncodeParameters(), &error);

        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVInputContext& FFAVTranscoderParameter::getInputContext() {
		return this->inputContext;
	}

    FFAVVideoEncodeParametersPtr FFAVTranscoderParameter::getVideoEncodeParameters() {
        return this->videoEncodeParameters;
    }

    FFAVAudioEncodeParametersPtr FFAVTranscoderParameter::getAudioEncodeParameters() {
        return this->audioEncodeParameters;
    }

    FFAVCodecContextPtr FFAVTranscoderParameter::getVideoDecodeContext() {
        return this->videoDecodeContext;
    }

    FFAVCodecContextPtr FFAVTranscoderParameter::getAudioDecodeContext() {
        return this->audioDecodeContext;
    }

    FFAVCodecContextPtr FFAVTranscoderParameter::getVideoEncodeContext() {
        return this->videoEncodeContext;
    }

    FFAVCodecContextPtr FFAVTranscoderParameter::getAudioEncodeContext() {
        return this->audioEncodeContext;
    }

    void FFAVTranscoderParameter::setHLSSegmentDuration(int duration) {
        this->hlsSegmentDuration = duration;
    }

    int FFAVTranscoderParameter::getHLSSegmentDuration() {
        return this->hlsSegmentDuration;
    }
}
