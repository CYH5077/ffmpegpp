#pragma once

#include "ffmpeg/AVType.hpp"
#include "ffmpeg/Rational.hpp"

namespace av {
    class Stream {
    public:
        explicit Stream();
        explicit Stream(AVStream* stream);
        virtual ~Stream();

    public: // getter setter
        bool isValidStream() const;

        const Rational& getTimebase() const;
        const Rational& getFramerate() const;

    public: // Raw pointer
        AVStream* getRawStream();
        AVCodecParameters* getRawCodecParameters();

        void setRawStream(AVStream* stream);

    private:
        AVStream* stream;

        Rational timebase;
        Rational framerate;
    };
};