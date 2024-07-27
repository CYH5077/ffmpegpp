#include "codecs/FFAVAudioEncodeParameters.hpp"

#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

namespace ff {
    FFAVAudioEncodeParametersPtr FFAVAudioEncodeParameters::create() {
        return std::make_shared<FFAVAudioEncodeParameters>();
    }

    FFAVAudioEncodeParametersPtr FFAVAudioEncodeParameters::create(FFAVStreamPtr stream) {
        return std::make_shared<FFAVAudioEncodeParameters>(stream);
    }

    FFAVAudioEncodeParametersPtr FFAVAudioEncodeParameters::create(FFAVAudioEncodeParametersPtr &encodeParameters) {
        return std::make_shared<FFAVAudioEncodeParameters>(encodeParameters);
    }


    FFAVAudioEncodeParameters::FFAVAudioEncodeParameters() {
        this->bitrate = 0;
        this->sampleRate = 0;
        this->timebase = FFAVRational();
        this->channelLayout = FFAVChannelLayout::create();
        this->sampleFormat = 0;
    }

    FFAVAudioEncodeParameters::FFAVAudioEncodeParameters(FFAVStreamPtr stream) {
        this->copyFrom(stream);
    }

    FFAVAudioEncodeParameters::FFAVAudioEncodeParameters(FFAVAudioEncodeParametersPtr &encodeParameters) {
        *this = *encodeParameters;
    }

    FFAVAudioEncodeParameters::~FFAVAudioEncodeParameters() {

    }

    void FFAVAudioEncodeParameters::copyFrom(FFAVStreamPtr stream) {
        AVStream* streamRaw = stream->getImpl()->getRaw();

        this->bitrate = streamRaw->codecpar->bit_rate;
        this->sampleFormat = streamRaw->codecpar->format;
        this->sampleRate = streamRaw->codecpar->sample_rate;

        FFAVRational rational(streamRaw->time_base.num, streamRaw->time_base.den);
        this->timebase = rational;

        this->channelLayout = stream->getChannelLayout();

        this->decodeStream = stream;
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

    FFAVStreamPtr FFAVAudioEncodeParameters::getDecodeStream() const {
		return this->decodeStream;
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

    void FFAVAudioEncodeParameters::setDecodeStream(FFAVStreamPtr stream) {
		this->decodeStream = stream;
	}
};