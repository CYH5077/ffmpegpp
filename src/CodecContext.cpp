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







bool createVideoDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result) {
    if (codecContext == nullptr ||
        result       == nullptr) {
        return false;
    }
    return createDecodeContext(demuxer.getVideoAVCodecID(), demuxer.getRawVideoCodecParameters(), codecContext, result);
}

bool createAudioDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result) {
    if (codecContext == nullptr ||
        result       == nullptr) {
        return false;
    }
    return createDecodeContext(demuxer.getAudioAVCodecID(), demuxer.getRawAudioCodecParameters(), codecContext, result);
}

bool createDecodeContext(int codecID, AVCodecParameters* codecParameters, CodecContext* codecContext, AVResult* result) {
    const AVCodec* decodeCodec = avcodec_find_decoder((AVCodecID)codecID);
    if (decodeCodec == nullptr) {
        return result->failed(AVERROR(EINVAL), "decoder not found");
    }

    AVCodecContext* decodeCodecContext = avcodec_alloc_context3(decodeCodec);
    if (decodeCodecContext == nullptr) {
        return result->avFailed(AVERROR(ENOMEM));
    }
    // 실패시 decodeCodecContext 를 해제하기 위함.
    CodecContext tempCodecContext(decodeCodecContext);

    int ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    ret = avcodec_open2(decodeCodecContext, decodeCodec, nullptr);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    *codecContext = std::move(tempCodecContext);
    return result->success();
}

bool createEncodeContext(const std::string& codecName, EncodeParameter& encodeParameter, CodecContext* codecContext, AVResult* result) {
    if (codecContext == nullptr ||
        result       == nullptr) {
        return false;
    }

    const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
    if (codec == nullptr) {
        return result->failed(-1, "Codec not found");
    }
    
    return createEncodeContext(codec, encodeParameter, codecContext, result);
}

bool createEncodeContext(CODEC_ID codecID, EncodeParameter& encodeParameter, CodecContext* codecContext, AVResult* result) {
    if (codecContext == nullptr ||
        result       == nullptr) {
        return false;
    }

    const AVCodec* codec = avcodec_find_encoder((AVCodecID)av::codecIDToAVCodecID(codecID));
    if (codec == nullptr) {
        return result->failed(-1, "Codec not found");
    }

    return createEncodeContext(codec, encodeParameter, codecContext, result);
}

bool createEncodeContext(const AVCodec* codec, EncodeParameter& encodeParameter, CodecContext* codecContext, AVResult* result) {
    AVCodecContext* encodeCodecContext = avcodec_alloc_context3(codec);
    if (encodeCodecContext == nullptr) {
        return result->avFailed(AVERROR(ENOMEM));
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
    CodecContext tempCodecContext(encodeCodecContext);

    int ret = avcodec_open2(encodeCodecContext, codec, nullptr);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    *codecContext = std::move(tempCodecContext);
    return result->success();
}
};