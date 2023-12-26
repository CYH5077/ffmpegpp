#pragma once

#include "AVType.hpp"
#include "Rational.hpp"
#include "ChannelLayout.hpp"

namespace av {
    class AudioEncodeParameters {
    public:
        explicit AudioEncodeParameters();
        virtual ~AudioEncodeParameters();

    public:
        long long       getBitrate();
        SAMPLE_FORMAT   getSampleFormat();
        const Rational& getTimebase();
        int             getSamplerate();
        const ChannelLayout& getChannelLayout();

        void setBitrate(long long bitrate);
        void setSampleFormat(SAMPLE_FORMAT sampleFormat);
        void setTimebase(const Rational& timebase);
        void setSamplerate(int samplerate);
        void setChannelLayout(const ChannelLayout& channelLayout);

    private:
        long long bitrate;

        SAMPLE_FORMAT sampleFormat;

        Rational timebase;

        int samplerate;

        ChannelLayout channelLayout;
    };
};