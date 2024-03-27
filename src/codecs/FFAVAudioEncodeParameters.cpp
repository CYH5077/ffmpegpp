#include "codecs/FFAVAudioEncodeParameters.hpp"

#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVAudioEncodeParametersPtr FFAVAudioEncodeParameters::create() {
        return std::make_shared<FFAVAudioEncodeParameters>();
    }

    FFAVAudioEncodeParametersPtr FFAVAudioEncodeParameters::create(FFAVInputContext &inputContext) {
        return std::make_shared<FFAVAudioEncodeParameters>(inputContext);
    }

    FFAVAudioEncodeParametersPtr FFAVAudioEncodeParameters::create(FFAVAudioEncodeParametersPtr &encodeParameters) {
        return std::make_shared<FFAVAudioEncodeParameters>(encodeParameters);
    }


    FFAVAudioEncodeParameters::FFAVAudioEncodeParameters() {
        this->bitrate = 0;
        this->sampleRate = 0;
        this->timebase = FFAVRational();
        this->channelLayout = FFAVChannelLayout::create();
    }

    FFAVAudioEncodeParameters::FFAVAudioEncodeParameters(FFAVInputContext &inputContext) {
        this->copyFrom(inputContext);
    }

    FFAVAudioEncodeParameters::FFAVAudioEncodeParameters(FFAVAudioEncodeParametersPtr &encodeParameters) {
        *this = *encodeParameters;
    }

    FFAVAudioEncodeParameters::~FFAVAudioEncodeParameters() {

    }

    void FFAVAudioEncodeParameters::copyFrom(FFAVInputContext &inputContext) {
        AVStream* stream = inputContext.getAudioStream()->getImpl()->getRaw();

        this->bitrate = stream->codecpar->bit_rate;
        this->sampleFormat = stream->codecpar->format;
        this->sampleRate   = stream->codecpar->sample_rate;

        FFAVRational rational(stream->time_base.num, stream->time_base.den);
        this->timebase = rational;

        this->channelLayout = inputContext.getAudioChannelLayout();
    }

    long long FFAVAudioEncodeParameters::getBitrate() const {
        return this->bitrate;
    }

    int FFAVAudioEncodeParameters::getSampleFormat() const {
        return this->sampleFormat;
    }

    int FFAVAudioEncodeParameters::getSamplerate() const {
        return this->sampleRate;
    }

    const FFAVRational& FFAVAudioEncodeParameters::getTimebase() const {
        return this->timebase;
    }

    const FFAVChannelLayoutPtr FFAVAudioEncodeParameters::getChannelLayout() const {
        return this->channelLayout;
    }

    void FFAVAudioEncodeParameters::setBitrate(long long bitrate) {
        this->bitrate = bitrate;
    }

    void FFAVAudioEncodeParameters::setSampleFormat(int sampleFormat) {
        this->sampleFormat = sampleFormat;
    }

    void FFAVAudioEncodeParameters::setSampleRate(int sampleRate) {
        this->sampleRate = sampleRate;
    }

    void FFAVAudioEncodeParameters::setTimebase(const FFAVRational& timebase) {
        this->timebase = timebase;
    }

    void FFAVAudioEncodeParameters::setChannelLayout(const FFAVChannelLayoutPtr &channelLayout) {
        this->channelLayout = channelLayout;
    }
};