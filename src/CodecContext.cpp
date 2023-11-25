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

CodecContext::CodecContext(CodecContext&& other) {
    this->codecContext = other.codecContext;
    other.codecContext = nullptr;
}

CodecContext::CodecContext(AVCodecContext* codecContext) {
    this->codecContext = codecContext;
}

CodecContext::~CodecContext() {
    if (this->codecContext != nullptr) {
        avcodec_free_context(&this->codecContext);
    }
}
void CodecContext::setAVCodecContext(AVCodecContext* codecContext) {
    this->codecContext = codecContext;
}

bool CodecContext::isVaildContext() {
    if (this->codecContext == nullptr) {
        return false;
    }   
    return true;
}

AVCodecContext* CodecContext::getRawCodecContext() {
    return this->codecContext;
}

CodecContext& CodecContext::operator=(CodecContext&& other) {
    if (this != &other) {
        this->codecContext = other.codecContext;
        other.codecContext = nullptr;
    }
    return *this;
}







CodecContextPtr createVideoDecodeContext(Demuxer& demuxer, AVResult* result) {
    if (result == nullptr) {
        return nullptr;
    }
    return createDecodeContext(demuxer.getVideoAVCodecID(), demuxer.getRawVideoCodecParameters(), result);
}

CodecContextPtr createAudioDecodeContext(Demuxer& demuxer, AVResult* result) {
    if (result == nullptr) {
        return nullptr;
    }
    return createDecodeContext(demuxer.getAudioAVCodecID(), demuxer.getRawAudioCodecParameters(), result);
}

CodecContextPtr createDecodeContext(int codecID, AVCodecParameters* codecParameters, AVResult* result) {
    CodecContextPtr codecContext;
    try {
        codecContext = std::make_shared<CodecContext>();
    } catch (std::bad_alloc& e) {
        result->failed(-1, e.what());
        return nullptr;
    }

    if (codecParameters == nullptr) {
        return codecContext;
    }

    const AVCodec* decodeCodec = avcodec_find_decoder((AVCodecID)codecID);
    if (decodeCodec == nullptr) {
        result->failed(AVERROR(EINVAL), "decoder not found");
        return codecContext;
    }

    AVCodecContext* decodeCodecContext = avcodec_alloc_context3(decodeCodec);
    if (decodeCodecContext == nullptr) {
        result->avFailed(AVERROR(ENOMEM));
        return codecContext;
    }
    // 실패시 decodeCodecContext 를 해제하기 위함.
    codecContext->setAVCodecContext(decodeCodecContext);

    int ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
    if (ret < 0) {
        result->avFailed(ret);
        return codecContext;
    }

    ret = avcodec_open2(decodeCodecContext, decodeCodec, nullptr);
    if (ret < 0) {
        result->avFailed(ret);
        return codecContext;
    }

    result->success();
    return codecContext;
}

CodecContextPtr createEncodeContext(const std::string& codecName, EncodeParameter& encodeParameter, AVResult* result) {
    if (result == nullptr) {
        return nullptr;
    }

    const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
    if (codec == nullptr) {
        result->failed(-1, "Codec not found");
        return nullptr;
    }
    
    return createEncodeContext(codec, encodeParameter, result);
}

CodecContextPtr createEncodeContext(CODEC_ID codecID, EncodeParameter& encodeParameter, AVResult* result) {
    if (result == nullptr) {
        return nullptr;
    }

    const AVCodec* codec = avcodec_find_encoder((AVCodecID)av::codecIDToAVCodecID(codecID));
    if (codec == nullptr) {
        result->failed(-1, "Codec not found");
        return nullptr;
    }

    return createEncodeContext(codec, encodeParameter, result);
}

CodecContextPtr createEncodeContext(const AVCodec* codec, EncodeParameter& encodeParameter, AVResult* result) {
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
        return codecContext;
    }
    encodeCodecContext->bit_rate = encodeParameter.getBitrate();
    encodeCodecContext->width    = encodeParameter.getWidth();
    encodeCodecContext->height   = encodeParameter.getHeight();
    encodeCodecContext->time_base = AVRational{encodeParameter.getTimeBase().getNum() , encodeParameter.getTimeBase().getDen()};
    encodeCodecContext->framerate = AVRational{encodeParameter.getFrameRate().getNum(), encodeParameter.getFrameRate().getDen()};
    encodeCodecContext->gop_size     = encodeParameter.getGOPSize();
    encodeCodecContext->max_b_frames = encodeParameter.getMaxBFrames();
    encodeCodecContext->pix_fmt      = (AVPixelFormat)av::pixelFormatToAVPixelFormat(encodeParameter.getPixelFormat());
    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(encodeCodecContext->priv_data, "preset", "slow", 0);
    }
    codecContext->setAVCodecContext(encodeCodecContext);
    
    int ret = avcodec_open2(encodeCodecContext, codec, nullptr);
    if (ret < 0) {
        result->avFailed(ret);
        return codecContext;
    }

    result->success();
    return codecContext;
}
};