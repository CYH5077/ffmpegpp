#pragma once

#include "AVType.hpp"
#include "Rational.hpp"

namespace av {

class VideoEncodeParameter {
public:
    explicit VideoEncodeParameter();
    virtual ~VideoEncodeParameter();

public: // getter setter
    unsigned long getBitrate();
    int getWidth();
    int getHeight();
    const Rational& getTimeBase();
    const Rational& getFrameRate();
    int getGOPSize();
    int getMaxBFrames();
    PIXEL_FORMAT getPixelFormat();
    int getThreadCount();

    void setBitrate(unsigned long bitrate);
    void setWidth(int width);
    void setHeight(int height);
    void setTimeBase(const Rational&& timebase);
    void setFrameRate(const Rational&& framerate);
    void setGOPSize(int gopSize);
    void setMaxBFrames(int maxBFrames);
    void setPixelFormat(PIXEL_FORMAT pixelFormat);
    void setThreadCount(int threadCount);

private:
    unsigned long bitrate;

    int width;
    int height;

    Rational timebase;
    Rational framerate;

    int gopSize;
    int maxBFrames;

    PIXEL_FORMAT pixelFormat;

    int threadCount;
};

};