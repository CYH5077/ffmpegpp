#pragma once

#include <iostream>
#include <memory>

#include "ffmpeg/AVType.hpp"
#include "ffmpeg/AVResult.hpp"
#include "ffmpeg/Rational.hpp"


namespace av {
    class Packet {
    public:
        explicit Packet();
        explicit Packet(AVPacket* packet, MEDIA_TYPE mediaType);
        virtual ~Packet();

    public:
        Packet(const Packet&) = delete;
        Packet& operator=(const Packet&) = delete;

    public:
        void unref();

        void rescaleTS(const Rational& preTimebase, const Rational& targetTimebase);

    public: // getter setter
        bool isValidPacket();

        int64_t getPTS();
        int64_t getDTS();
        int     getSize();
        int     getStreamIndex();
        double  getPTSTimeToSecond(const Rational&& timebase);
        MEDIA_TYPE getMediaType();
        long long  getFrameNumber();

        void setPTS(int64_t pts);
        void setDTS(int64_t dts);
        void setPos(int pos);
        void setStreamIndex(int streamIndex);
        void setMediaType(MEDIA_TYPE mediaType);
        void setFrameNumber(long long frameNumber);

    public: // Raw pointer
        AVPacket* getRawPacket();

        void setRawPacket(AVPacket* packet);

    private:
        AVPacket* packet;

        MEDIA_TYPE mediaType;

        long long frameNumber;
    };
};