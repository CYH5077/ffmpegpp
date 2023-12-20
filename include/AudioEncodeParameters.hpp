#pragma once

#include "AVType.hpp"
#include "Rational.hpp"

namespace av {
    class AudioEncodeParameters {
    public:
        explicit AudioEncodeParameters();
        virtual ~AudioEncodeParameters();

    public:
        long long       getBitrate();
        SAMPLE_FORMAT   getSampleFormat();
        const Rational& getTimebase();

        void setBitrate(long long bitrate);
        void setSampleFormat(SAMPLE_FORMAT sampleFormat);
        void setTimebase(const Rational& timebase);

    private:
        long long bitrate;

        SAMPLE_FORMAT sampleFormat;

        Rational timebase;
    };
};