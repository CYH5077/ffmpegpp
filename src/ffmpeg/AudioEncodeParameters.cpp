#include "ffmpeg/AudioEncodeParameters.hpp"

namespace av {
    AudioEncodeParameters::AudioEncodeParameters() {
        this->bitrate = 0;

        this->sampleFormat = SAMPLE_FORMAT::NONE;

        this->samplerate = 0;
    }

    AudioEncodeParameters::~AudioEncodeParameters() {

    }

    long long AudioEncodeParameters::getBitrate() {
        return this->bitrate;
    }

    SAMPLE_FORMAT AudioEncodeParameters::getSampleFormat() {
        return this->sampleFormat;
    }

    const Rational& AudioEncodeParameters::getTimebase() {
        return this->timebase;
    }

    int AudioEncodeParameters::getSamplerate() {
        return this->samplerate;
    }

    const ChannelLayout& AudioEncodeParameters::getChannelLayout() {
        return this->channelLayout;
    }

    void AudioEncodeParameters::setBitrate(long long bitrate) {
        this->bitrate = bitrate;
    }

    void AudioEncodeParameters::setSampleFormat(av::SAMPLE_FORMAT sampleFormat) {
        this->sampleFormat = sampleFormat;
    }

    void AudioEncodeParameters::setTimebase(const av::Rational& timebase) {
        this->timebase = timebase;
    }

    void AudioEncodeParameters::setSamplerate(int samplerate) {
        this->samplerate = samplerate;
    }
    void AudioEncodeParameters::setChannelLayout(const av::ChannelLayout& channelLayout) {
        this->channelLayout = channelLayout;
    }
};
