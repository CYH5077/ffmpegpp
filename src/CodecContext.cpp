#include "CodecContext.hpp"

namespace av {

CodecContext::CodecContext() {
    this->codecContext = nullptr;
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

bool createVideoDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result) {
    if (codecContext == nullptr ||
        result       == nullptr) {
        return false;
    }
    return createDecodeContext(demuxer.getVideoCodecID(), demuxer.getRawVideoCodecParameters(), codecContext, result);
}

bool createAudioDecodeContext(Demuxer& demuxer, CodecContext* codecContext, AVResult* result) {
    if (codecContext == nullptr ||
        result       == nullptr) {
        return false;
    }
    return createDecodeContext(demuxer.getAudioCodecID(), demuxer.getRawAudioCodecParameters(), codecContext, result);
}

bool createDecodeContext(AVCodecID codecID, AVCodecParameters* codecParameters, CodecContext* codecContext, AVResult* result) {
    const AVCodec* decodeCodec = avcodec_find_decoder(codecID);
    if (decodeCodec == nullptr) {
        return result->failed(AVERROR(EINVAL), "decoder not found");
    }

    AVCodecContext* decodeCodecContext = avcodec_alloc_context3(decodeCodec);
    if (decodeCodecContext == nullptr) {
        return result->avFailed(AVERROR(ENOMEM));
    }
    codecContext->setAVCodecContext(decodeCodecContext);

    int ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    ret = avcodec_open2(decodeCodecContext, decodeCodec, nullptr);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    return result->success();
}


};