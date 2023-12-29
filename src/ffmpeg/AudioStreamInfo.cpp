#include "ffmpeg/AudioStreamInfo.hpp"

namespace av {
    std::shared_ptr<AudioStreamInfo> AudioStreamInfo::createAudioStreamInfo(av::Stream& stream, av::CodecParameters& codecParameters) {
        std::shared_ptr<AudioStreamInfo> streamInfo = nullptr;
        if (stream.isValidStream()                   == true ||
            codecParameters.isValidCodecParameters() == true) {
            streamInfo = std::make_shared<AudioStreamInfo>(stream, codecParameters);
        }
        return streamInfo;
    }

    AudioStreamInfo::AudioStreamInfo(av::Stream& stream, av::CodecParameters& codecParameters) {
        this->stream = stream;
        this->codecParameters = codecParameters;
    }

    AudioStreamInfo::~AudioStreamInfo() {

    }

    long long AudioStreamInfo::getBitrate() {
        return this->codecParameters.getBitrate();
    }

    int AudioStreamInfo::getSamplerate() {
        return this->codecParameters.getSamplerate();
    }

    const Rational& AudioStreamInfo::getTimebase() {
        return this->stream.getTimebase();
    }

    const ChannelLayout& AudioStreamInfo::getChannelLayout() {
        return this->codecParameters.getChannelLayout();
    }


};