#pragma once

#include "AVType.hpp"
#include "Rational.hpp"

namespace av {
    class VideoEncodeParameters {
    public:
        explicit VideoEncodeParameters();
        virtual ~VideoEncodeParameters();

    public: // getter setter
        unsigned long getBitrate();
        int getWidth();
        int getHeight();
        int getGOPSize();
        int getMaxBFrames();
        int getEncodeThreadCount();
        PIXEL_FORMAT    getPixelFormat();
        const Rational& getTimeBase();

        void setBitrate(long long bitrate);
        void setWidth(int width);
        void setHeight(int height);
        void setTimeBase(const Rational& timebase);
        void setGOPSize(int gopSize);
        void setMaxBFrames(int maxBFrames);
        void setPixelFormat(PIXEL_FORMAT pixelFormat);
        void setEncodeThreadCount(int threadCount);

    private:
        long long bitrate;

        int width;
        int height;

        int gopSize;
        int maxBFrames;

        Rational timebase;

        PIXEL_FORMAT pixelFormat;

        int encodeThreadCount;
    };
};