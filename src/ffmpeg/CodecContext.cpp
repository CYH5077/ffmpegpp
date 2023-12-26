#include "ffmpeg/CodecContext.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/hwcontext.h"
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
        return AVMediaTypeToMediaType((int)this->codecContext->codec->type);
    }

    bool CodecContext::isVaildContext() {
        if (this->codecContext == nullptr) {
            return false;
        }
        return true;
    }

    bool CodecContext::isCUDAContext() {
        const AVCodecHWConfig* codecHwConfig = nullptr;
        for (int i = 0; (codecHwConfig = avcodec_get_hw_config(this->codecContext->codec, i)); i++) {
            if (codecHwConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                codecHwConfig->device_type == AV_HWDEVICE_TYPE_CUDA) {
                this->cudaDecodeHWFormat = (int)codecHwConfig->pix_fmt;
                return true;
            }
        }
        return false;
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

    int CodecContext::getRawHWFormat() {
        return this->cudaDecodeHWFormat;
    }


    ////////////////////////////   Create Decode Context

    static CodecContextPtr createAVDecodeContext(int codecID, AVCodecParameters* codecParameters, bool cudaEnable, AVResult* result) {
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

        int ret = 0;
        if (cudaEnable == true) {
            AVBufferRef* hwDeviceCtx = nullptr;
            ret = av_hwdevice_ctx_create(&decodeCodecContext->hw_device_ctx, AV_HWDEVICE_TYPE_CUDA, nullptr, nullptr, 0);
            if (ret < 0) {
                result->avFailed(ret);
                return nullptr;
            }
        }

        ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
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
        return createAVDecodeContext(codecParameters.getCodecID(), codecParameters.getRawCodecParameters(), false, result);
    }

    CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        CodecParameters codecParameters = demuxer.getAudioCodecParameters();
        return createAVDecodeContext(codecParameters.getCodecID(), codecParameters.getRawCodecParameters(), false, result);
    }


    //////////////////////////////////// Create Video Encode Context


    static CodecContextPtr createAVVideoEncodeContext(const AVCodec* codec, VideoEncodeParameters& encodeParameter, bool cudaEnable, AVResult* result) {
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
        encodeCodecContext->pix_fmt      = (AVPixelFormat)pixelFormatToAVPixelFormat(encodeParameter.getPixelFormat());
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

    CodecContextPtr createVideoEncodeContext(const std::string&& codecName, VideoEncodeParameters& encodeParameter, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameter, false, result);
    }

    CodecContextPtr createVideoEncodeContext(VIDEO_CODEC_ID codecID, VideoEncodeParameters& encodeParameter, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder((AVCodecID)videoCodecIDToAVCodecID(codecID));
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameter, false, result);
    }



    /////////////////// Create Audio Encode Context


    static bool isValidSampleFormat(const AVCodec* codec, SAMPLE_FORMAT sampleFormat) {
        enum AVSampleFormat avSampleFormat = (enum AVSampleFormat)sampleFormatToAVSampleFormat(sampleFormat);
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
        encodeCodecContext->sample_fmt  = (AVSampleFormat)sampleFormatToAVSampleFormat(encodeParameters.getSampleFormat());
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

    CodecContextPtr  createAudioEncodeContext(const std::string&& codecName, AudioEncodeParameters& encodeParameter, AVResult* result) {
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

    CodecContextPtr createAudioEncodeContext(AUDIO_CODEC_ID codecID, AudioEncodeParameters& encodeParameters, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder((AVCodecID)audioCodecIDToAVCodecID(codecID));
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVAudioEncodeContext(codec, encodeParameters, result);
    }


    CodecContextPtr createVideoCUDADecodeContext(Demuxer& demuxer, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        CodecParameters codecParameters = demuxer.getVideoCodecParameters();
        return createAVDecodeContext(codecParameters.getCodecID(), codecParameters.getRawCodecParameters(), true, result);
    }

    CodecContextPtr createVideoCUDAEncoderContext(VIDEO_CODEC_ID codecID, VideoEncodeParameters& encodeParameters, AVResult* result) {
        if (result == nullptr) {
            return nullptr;
        }

        const AVCodec* codec = avcodec_find_encoder((AVCodecID)videoCodecIDToAVCodecID(codecID));
        if (codec == nullptr) {
            result->failed(-1, "Codec not found");
            return nullptr;
        }

        return createAVVideoEncodeContext(codec, encodeParameters, true, result);
    }

};