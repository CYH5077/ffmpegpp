#include "type/FFAVEncodeStream.hpp"

#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVEncodeStream::FFAVEncodeStream(DATA_TYPE type) : FFAVStream(type) {
        this->videoCodec = VIDEO_CODEC::NONE;
        this->audioCodec = AUDIO_CODEC::NONE;
        FFAVStream::streamImpl = FFAVStreamImpl::create();
    }

    FFAVEncodeStream::~FFAVEncodeStream() {}

    void FFAVEncodeStream::setCodec(VIDEO_CODEC codec) {
        this->videoCodec = codec;
    }

    void FFAVEncodeStream::setCodec(AUDIO_CODEC codec) {
        this->audioCodec = codec;
    }

    bool FFAVEncodeStream::isVideoStream() {
        return this->videoCodec != VIDEO_CODEC::NONE;
    }

    bool FFAVEncodeStream::isAudioStream() {
        return this->audioCodec != AUDIO_CODEC::NONE;
    }

    VIDEO_CODEC FFAVEncodeStream::getVideoCodec() {
        return this->videoCodec;
    }

    AUDIO_CODEC FFAVEncodeStream::getAudioCodec() {
        return this->audioCodec;
    }

    AVError FFAVEncodeStream::encode() {
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}
