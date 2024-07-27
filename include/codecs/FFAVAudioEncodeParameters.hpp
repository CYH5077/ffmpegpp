#pragma once

#include "type/ffav.hpp"

namespace ff {
    class FFAVAudioEncodeParameters;
    typedef std::shared_ptr<FFAVAudioEncodeParameters> FFAVAudioEncodeParametersPtr;

    class FFAVAudioEncodeParameters {
    public:
        explicit FFAVAudioEncodeParameters();
        explicit FFAVAudioEncodeParameters(FFAVStreamPtr stream);
        explicit FFAVAudioEncodeParameters(FFAVAudioEncodeParametersPtr& encodeParameters);

        virtual ~FFAVAudioEncodeParameters();

    public:
        static FFAVAudioEncodeParametersPtr create();
        static FFAVAudioEncodeParametersPtr create(FFAVStreamPtr stream);
        static FFAVAudioEncodeParametersPtr create(FFAVAudioEncodeParametersPtr& encodeParameters);

    public:
        void copyFrom(FFAVStreamPtr stream);

        long long getBitrate() const;
        int getSampleFormat() const;
        int getSamplerate() const;
        const FFAVRational& getTimebase() const;
        const FFAVChannelLayoutPtr getChannelLayout() const;
        FFAVStreamPtr getDecodeStream() const;

        void setBitrate(long long bitrate);
        void setSampleFormat(int sampleFormat);
        void setSampleRate(int sampleRate);
        void setTimebase(const FFAVRational& timebase);
        void setChannelLayout(const FFAVChannelLayoutPtr& channelLayout);
        void setDecodeStream(FFAVStreamPtr stream);

    private:
        long long bitrate;

        int sampleFormat;
        int sampleRate;

        FFAVStreamPtr decodeStream;

        FFAVRational timebase;

        FFAVChannelLayoutPtr channelLayout;
    };
};
