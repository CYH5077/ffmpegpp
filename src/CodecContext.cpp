#include "CodecContext.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
}

#include <utility>

namespace av {

    CodecContext::CodecContext() {
        this->codecContext = nullptr;
    }

    CodecContext::CodecContext(AVCodecContext* codecContext) {
        this->setRawCodeContext(codecContext);
    }

    CodecContext::~CodecContext() {
        if (this->codecContext != nullptr) {
            avcodec_free_context(&this->codecContext);
        }
    }

    int64_t CodecContext::getBitrate() {
        return this->codecContext->bit_rate;
    }

    const Rational& CodecContext::getTimebase() {
        return this->timebase;
    }

    const Rational& CodecContext::getFramerate() {
        return this->framerate;
    }

    MEDIA_TYPE CodecContext::getMediaType() {
        return av::AVMediaTypeToMediaType((int)this->codecContext->codec->type);
    }

    bool CodecContext::isVaildContext() {
        if (this->codecContext == nullptr) {
            return false;
        }
        return true;
    }

    void CodecContext::setAVCodecContext(AVCodecContext* codecContext) {
        this->codecContext = codecContext;
    }

    void CodecContext::setRawCodeContext(AVCodecContext *codecContext) {
        this->codecContext = codecContext;
        this->timebase     = Rational(codecContext->time_base.num, codecContext->time_base.den);
        this->framerate    = Rational(codecContext->framerate.num, codecContext->framerate.den);
    }
    AVCodecContext* CodecContext::getRawCodecContext() {
        return this->codecContext;
    }


    ////////////////////////////   Create Decode Context

    static CodecContextPtr createAVDecodeContext(int codecID, AVCodecParameters* codecParameters, AVResult* result) {
        CodecContextPtr codecContext;
        try {
            codecContext = std::make_shared<CodecContext>();
        } catch (std::bad_alloc& e) {
            result->failed(-1, e.what());
            return nullptr;
        }

        if (codecParameters == nullptr) {
            return nullptr;
        }

        const AVCodec* decodeCodec = avcodec_find_decoder((AVCodecID)codecID);
        if (decodeCodec == nullptr) {
            result->failed(AVERROR(EINVAL), "decoder not found");
            return nullptr;
        }

        AVCodecContext* decodeCodecContext = avcodec_alloc_context3(decodeCodec);
        if (decodeCodecContext == nullptr) {
            result->avFailed(AVERROR(ENOMEM));
            return nullptr;
        }
        // 실패시 decodeCodecContext 를 해제하기 위함.
        codecContext->setAVCodecContext(decodeCodecContext);

        int ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
        if (ret < 0) {
            result->avFailed(ret);
            return nullptr;
        }

        ret = avcodec_open2(decodeCodecContext, decodeCodec, nullptr);
        if (ret < 0) {
            result->avFailed(ret);
            return nullptr;
        }

        result->success();
        return codecContext;
    }

    CodecContextPtr createVideoDecodeContext(Demuxer& demuxer, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        CodecParameters codecParameters = demuxer.getVideoCodecParameters();
        return createAVDecodeContext(codecParameters.getCodecID(), codecParameters.getRawCodecParameters(), result);
    }

    CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        CodecParameters codecParameters = demuxer.getAudioCodecParameters();
        return createAVDecodeContext(codecParameters.getCodecID(), codecParameters.getRawCodecParameters(), result);
    }


    //////////////////////////////////// Create Video Encode Context


    static CodecContextPtr createAVVideoEncodeContext(const AVCodec* codec, VideoEncodeParameters& encodeParameter, AVResult* result) {
        CodecContextPtr codecContext;
        try {
            codecContext = std::make_shared<CodecContext>();
        } catch (std::bad_alloc& e) {
            result->failed(-1, e.what());
            return nullptr;
        }

        AVCodecContext* encodeCodecContext = avcodec_alloc_context3(codec);
        if (encodeCodecContext == nullptr) {
            result->avFailed(AVERROR(ENOMEM));
            return nullptr;
        }
        encodeCodecContext->bit_rate = encodeParameter.getBitrate();
        encodeCodecContext->width    = encodeParameter.getWidth();
        encodeCodecContext->height   = encodeParameter.getHeight();
        encodeCodecContext->time_base = AVRational{encodeParameter.getTimeBase().getNum() , encodeParameter.getTimeBase().getDen()};
        encodeCodecContext->framerate = AVRational{encodeParameter.getFrameRate().getNum(), encodeParameter.getFrameRate().getDen()};
        encodeCodecContext->gop_size     = encodeParameter.getGOPSize();
        encodeCodecContext->max_b_frames = encodeParameter.getMaxBFrames();
        encodeCodecContext->pix_fmt      = (AVPixelFormat)av::pixelFormatToAVPixelFormat(encodeParameter.getPixelFormat());
        encodeCodecContext->thread_count = encodeParameter.getEncodeThreadCount();
        if (codec->id == AV_CODEC_ID_H264) {
            av_opt_set(encodeCodecContext->priv_data, "preset", "slow", 0);
        }
        codecContext->setAVCodecContext(encodeCodecContext);

        int ret = avcodec_open2(encodeCodecContext, codec, nullptr);
        if (ret < 0) {
            result->avFailed(ret);
            return nullptr;
        }

        result->success();
        return codecContext;
    }

    CodecContextPtr createVideoEncodeContext(const std::string& codecName, VideoEncodeParameters& encodeParameter, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameter, result);
    }

    CodecContextPtr createVideoEncodeContext(CODEC_ID codecID, VideoEncodeParameters& encodeParameter, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder((AVCodecID)av::codecIDToAVCodecID(codecID));
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameter, result);
    }



    /////////////////// Create Audio Encode Context


    static bool isValidSampleFormat(const AVCodec* codec, SAMPLE_FORMAT sampleFormat) {
        enum AVSampleFormat avSampleFormat = (enum AVSampleFormat)av::sampleFormatToAVSampleFormat(sampleFormat);
        const enum AVSampleFormat* p = codec->sample_fmts;
        while (*p != AV_SAMPLE_FMT_NONE) {
            if (*p == avSampleFormat) {
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
/*
    static int copyChannelLayout(const AVCodec* codec, AVChannelLayout* dst) {
        const AVChannelLayout* p;
        const AVChannelLayout* bestChannelLayout;

        if (codec->ch_layouts == nullptr) {
            *dst = AV_CHANNEL_LAYOUT_STEREO;
            return 0;
        }

        int bestNBChannels = 0;
        p = codec->ch_layouts;
        while (p->nb_channels) {
            int nbChannels = p->nb_channels;
            if (nbChannels > bestNBChannels) {
                bestChannelLayout = p;
                bestNBChannels = nbChannels;
            }
            p++;
        }

        return av_channel_layout_copy(dst, bestChannelLayout);
    }
*/
    static CodecContextPtr createAVAudioEncodeContext(const AVCodec* codec, AudioEncodeParameters& encodeParameters, AVResult* result) {
        CodecContextPtr codecContext;
        try {
            codecContext = std::make_shared<CodecContext>();
        } catch (std::bad_alloc& e) {
            result->failed(-1, e.what());
            return nullptr;
        }

        AVCodecContext* encodeCodecContext = avcodec_alloc_context3(codec);
        if (encodeCodecContext == nullptr) {
            result->avFailed(AVERROR(ENOMEM));
            return nullptr;
        }
        codecContext->setAVCodecContext(encodeCodecContext);

        // 지원하는 SAMPLE_FORMAT 인지 확인.
        if (isValidSampleFormat(codec, encodeParameters.getSampleFormat()) == false) {
            result->failed(-1, "Not support SAMPLE_FORMAT");
        }

        ChannelLayout channelLayout = encodeParameters.getChannelLayout();
        int             samplerate  = encodeParameters.getSamplerate();
        const Rational& timebase    = encodeParameters.getTimebase();
        encodeCodecContext->time_base   = AVRational {timebase.getNum(), timebase.getDen()};
        encodeCodecContext->bit_rate    = encodeParameters.getBitrate();
        encodeCodecContext->sample_fmt  = (AVSampleFormat)av::sampleFormatToAVSampleFormat(encodeParameters.getSampleFormat());
        encodeCodecContext->sample_rate = samplerate > 0 ? samplerate : getBestSamplerate(codec);
        encodeCodecContext->ch_layout = *channelLayout.getRawChannelLayout();
        /*int ret = copyChannelLayout(codec, &encodeCodecContext->ch_layout);
        if (ret < 0) {
            result->avFailed(ret);
            return nullptr;
        }*/

        int ret = avcodec_open2(encodeCodecContext, codec, nullptr);
        if (ret < 0) {
            result->avFailed(ret);
            return nullptr;
        }

        result->success();
        return codecContext;
    }

    CodecContextPtr  createAudioEncodeContext(const std::string codecName, AudioEncodeParameters& encodeParameter, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVAudioEncodeContext(codec, encodeParameter, result);

    }

    CodecContextPtr createAudioEncodeContext(CODEC_ID codecID, AudioEncodeParameters& encodeParameters, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder((AVCodecID)av::codecIDToAVCodecID(codecID));
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVAudioEncodeContext(codec, encodeParameters, result);
    }

};