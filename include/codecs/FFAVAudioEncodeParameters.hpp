#pragma once

#include "type/ffav.hpp"

namespace ff {
    class FFAVAudioEncodeParameters;
    typedef std::shared_ptr<FFAVAudioEncodeParameters> FFAVAudioEncodeParametersPtr;

    class FFAVAudioEncodeParameters {
    public:
        explicit FFAVAudioEncodeParameters();
        explicit FFAVAudioEncodeParameters(FFAVInputContext &inputContext);
        explicit FFAVAudioEncodeParameters(FFAVAudioEncodeParametersPtr &encodeParameters);

        virtual ~FFAVAudioEncodeParameters();

    public:
        static FFAVAudioEncodeParametersPtr create();
        static FFAVAudioEncodeParametersPtr create(FFAVInputContext &inputContext);
        static FFAVAudioEncodeParametersPtr create(FFAVAudioEncodeParametersPtr &encodeParameters);

    public:
        void copyFrom(FFAVInputContext& inputContext);

        long long getBitrate() const;
        int       getSampleFormat() const;
        int       getSamplerate() const;
        const FFAVRational&        getTimebase() const;
        const FFAVChannelLayoutPtr getChannelLayout() const;

        void setBitrate(long long bitrate);
        void setSampleFormat(int sampleFormat);
        void setSampleRate(int sampleRate);
        void setTimebase(const FFAVRational& timebase);
        void setChannelLayout(const FFAVChannelLayoutPtr &channelLayout);

    private:
        long long bitrate;

        int sampleFormat;
        int sampleRate;

        FFAVRational timebase;

        FFAVChannelLayoutPtr channelLayout;
    };
};