#include "ffmpeg/CodecParameters.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace av {

    CodecParameters::CodecParameters() {
        this->codecParameters = nullptr;
    }

    CodecParameters::CodecParameters(AVCodecParameters *codecParameters) {
        this->setRawCodecParameters(codecParameters);
    }

    CodecParameters::~CodecParameters() {

    }

    bool CodecParameters::isValidCodecParameters() const {
        if (this->codecParameters == nullptr) {
            return false;
        }
        return true;
    }

    long long CodecParameters::getBitrate() const {
        return this->codecParameters->bit_rate;
    }

    int CodecParameters::getWidth() const {
        return this->codecParameters->width;
    }

    int CodecParameters::getHeight() const {
        return this->codecParameters->height;
    }

    int CodecParameters::getSamplerate() const {
        return this->codecParameters->sample_rate;
    }

    int CodecParameters::getCodecID() const {
        return this->codecParameters->codec_id;
    }

    const ChannelLayout& CodecParameters::getChannelLayout() const {
        return this->channelLayout;
    }

    AVCodecParameters* CodecParameters::getRawCodecParameters() const {
        return this->codecParameters;
    }

    void CodecParameters::setRawCodecParameters(AVCodecParameters *codecParameters) {
        this->codecParameters = codecParameters;

        this->channelLayout = ChannelLayout(&this->codecParameters->ch_layout);
    }

}