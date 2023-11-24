#pragma once

extern "C" {
    #include "libavformat/avformat.h"
}

#include "Rational.hpp"

namespace av {

class EncodeParameter {
public:
    explicit EncodeParameter();
    virtual ~EncodeParameter();

public: // getter setter
    uint64_t getBitrate();
    int      getWidth();
    int      getHeight();
    const Rational& getTimeBase();
    const Rational& getFrameRate();
    int      getGOPSize();
    int      getMaxBFrames();
    AVPixelFormat getPixelFormat();

    void setBitrate(uint64_t bitrate);
    void setWidth(int width);
    void setHeight(int height);
    void setTimeBase(const Rational&& timebase);
    void setFrameRate(const Rational&& framerate);
    void setGOPSize(int gopSize);
    void setMaxBFrames(int maxBFrames);
    void setPixelFormat(AVPixelFormat pixelFormat);

private:
    uint64_t bitrate;

    int width;
    int height;

    Rational timebase;
    Rational framerate;

    int gopSize;
    int maxBFrames;

    AVPixelFormat pixelFormat;
};

};