#include "codecs/createEncodeContext.hpp"

#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"
#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/hwcontext.h"
#include "libavutil/opt.h"
}

namespace ff {
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

    FFAVCodecContextPtr createAVVideoEncodeContext(const AVCodec* codec, FFAVVideoEncodeParametersPtr& encodeParameter, AVError* result) {
        FFAVCodecContextPtr codecContext;
        try {
            codecContext = FFAVCodecContext::create();
        } catch (std::bad_alloc& e) {
            result->setError(AV_ERROR_TYPE::FAILED, "FFAVCodecContext::create failed", -1, "");
            return nullptr;
        }

        AVCodecContext* encodeCodecContext = avcodec_alloc_context3(codec);
        if (encodeCodecContext == nullptr) {
            result->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_alloc_context3 failed", AVERROR(ENOMEM), "avcodec_alloc_context3");
            return nullptr;
        }

        encodeCodecContext->bit_rate = encodeParameter->getBitrate();
        encodeCodecContext->width    = encodeParameter->getWidth();
        encodeCodecContext->height   = encodeParameter->getHeight();
        encodeCodecContext->time_base = AVRational {encodeParameter->getTimeBase().getNum() , encodeParameter->getTimeBase().getDen()};
        encodeCodecContext->framerate = av_inv_q(encodeCodecContext->time_base);
        encodeCodecContext->gop_size     = encodeParameter->getGOPSize();
        encodeCodecContext->max_b_frames = encodeParameter->getMaxBFrames();
        encodeCodecContext->pix_fmt      = (AVPixelFormat)(encodeParameter->getPixelFormat());
        encodeCodecContext->thread_count = encodeParameter->getEncodeThreadCount();
    

        codecContext->getImpl()->setRaw(encodeCodecContext);


        int ret = avcodec_open2(encodeCodecContext, codec, nullptr);
        if (ret < 0) {
            result->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_open2 failed", ret, "avcodec_open2");
            return nullptr;
        }

        *result = AVError(AV_ERROR_TYPE::SUCCESS);
        return codecContext;
    }


    ///////// Audio
    static bool isValidSampleFormat(const AVCodec* codec, AVSampleFormat sampleFormat) {
        const enum AVSampleFormat* p = codec->sample_fmts;
        while (*p != AV_SAMPLE_FMT_NONE) {
            if (*p == sampleFormat) {
                return true;
            }
            p++;
        }
        return false;
    }

    static int getBestSamplerate(const AVCodec* codec) {
        const int* p = codec->supported_samplerates;
        int bestSamplerate = 0;

        if (p == nullptr) {
            return 44100;
        }

        while (*p) {
            if (!bestSamplerate || std::abs(44100 - *p) < std::abs(44100 - bestSamplerate)) {
                bestSamplerate = *p;
            }
            p++;
        }
        return bestSamplerate;
    }

    FFAVCodecContextPtr createAVAudioEncodeContext(const AVCodec* codec, FFAVAudioEncodeParametersPtr encodeParameters, AVError* result) {
        FFAVCodecContextPtr codecContext;
        try {
            codecContext = FFAVCodecContext::create();
        } catch (std::bad_alloc& e) {
            result->setError(AV_ERROR_TYPE::FAILED, "FFAVCodecContext::create failed", -1, "");
            return nullptr;
        }

        AVCodecContext* encodeCodecContext = avcodec_alloc_context3(codec);
        if (encodeCodecContext == nullptr) {
            result->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_alloc_context3 failed", AVERROR(ENOMEM), "avcodec_alloc_context3");
            return nullptr;
        }
        codecContext->getImpl()->setRaw(encodeCodecContext);

        // 지원하는 SAMPLE_FORMAT 인지 확인.
        if (isValidSampleFormat(codec, (AVSampleFormat)encodeParameters->getSampleFormat()) == false) {
            result->setError(AV_ERROR_TYPE::FAILED, "isValidSampleFormat failed (Not support SAMPLE_FORMAT)", -1 ,"");
            return nullptr;
        }

        FFAVChannelLayoutPtr channelLayout = encodeParameters->getChannelLayout();
        int samplerate               = encodeParameters->getSamplerate();
        const FFAVRational& timebase = encodeParameters->getTimebase();
        encodeCodecContext->time_base   = AVRational {timebase.getNum(), timebase.getDen()};
        encodeCodecContext->bit_rate    = encodeParameters->getBitrate();
        encodeCodecContext->sample_fmt  = (AVSampleFormat)encodeParameters->getSampleFormat();
        encodeCodecContext->sample_rate = samplerate > 0 ? samplerate : getBestSamplerate(codec);
        encodeCodecContext->ch_layout   = channelLayout->getImpl()->getRaw();
        /*int ret = copyChannelLayout(codec, &encodeCodecContext->ch_layout);
        if (ret < 0) {
            result->avFailed(ret);
            return nullptr;
        }*/

        int ret = avcodec_open2(encodeCodecContext, codec, nullptr);
        if (ret < 0) {
            result->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_open2 failed", ret, "avcodec_open2");
            return nullptr;
        }

        *result = AVError(AV_ERROR_TYPE::SUCCESS);
        return codecContext;
    }
};

namespace ff::video::encode {
    FFAVCodecContextPtr createCodecContext(std::string&& codecName, FFAVVideoEncodeParametersPtr encodeParameters, AVError* error) {
        if (error == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
        if (codec == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", AVERROR_ENCODER_NOT_FOUND, "avcodec_find_encoder_by_name");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameters, error);
    }

    FFAVCodecContextPtr createCodecContext(VIDEO_CODEC codec, FFAVVideoEncodeParametersPtr encodeParameters, AVError* error) {
        if (error == nullptr) {
            return nullptr;
        }

        const AVCodec* avCodec = avcodec_find_encoder_by_name(VIDEO_CODEC_TO_STRING(codec).c_str());
        if (avCodec == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", AVERROR_ENCODER_NOT_FOUND, "avcodec_find_encoder_by_name");
            return nullptr;
        }

        return createAVVideoEncodeContext(avCodec, encodeParameters, error);
    }

    FFAVCodecContextPtr createCUDACodecContext(HW_VIDEO_CODEC hwVideoCodec, FFAVVideoEncodeParametersPtr encodeParameters, AVError* error) {
        if (error == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder_by_name(HW_VIDEO_CODEC_TO_STRING(hwVideoCodec).c_str());
        if (codec == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", AVERROR_ENCODER_NOT_FOUND, "avcodec_find_encoder_by_name");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameters, error);
    }

};

namespace ff::audio::encode {
    FFAVCodecContextPtr createCodecContext(std::string& codecName, FFAVAudioEncodeParametersPtr encodeParameters, AVError* error) {
        if (error == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
        if (codec == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", AVERROR_ENCODER_NOT_FOUND, "avcodec_find_encoder_by_name");
            return nullptr;
        }

        return createAVAudioEncodeContext(codec, encodeParameters, error);
    }

    FFAVCodecContextPtr createCodecContext(AUDIO_CODEC codec, FFAVAudioEncodeParametersPtr encodeParameters, AVError* error) {
        if (error == nullptr) {
            return nullptr;
        }

        const AVCodec* avCodec = avcodec_find_encoder_by_name(AUDIO_CODEC_TO_STRING(codec).c_str());
        if (avCodec == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", AVERROR_ENCODER_NOT_FOUND, "avcodec_find_encoder_by_name");
            return nullptr;
        }

        return createAVAudioEncodeContext(avCodec, encodeParameters, error);
    }
};