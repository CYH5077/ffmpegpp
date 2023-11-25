#pragma once

#include "AVType.hpp"
#include "Rational.hpp"

namespace av {

class EncodeParameter {
public:
    explicit EncodeParameter();
    virtual ~EncodeParameter();

public: // getter setter
    unsigned long getBitrate();
    int      getWidth();
    int      getHeight();
    const Rational& getTimeBase();
    const Rational& getFrameRate();
    int      getGOPSize();
    int      getMaxBFrames();
    PIXEL_FORMAT getPixelFormat();

    void setBitrate(unsigned long bitrate);
    void setWidth(int width);
    void setHeight(int height);
    void setTimeBase(const Rational&& timebase);
    void setFrameRate(const Rational&& framerate);
    void setGOPSize(int gopSize);
    void setMaxBFrames(int maxBFrames);
    void setPixelFormat(PIXEL_FORMAT pixelFormat);

private:
    unsigned long bitrate;

    int width;
    int height;

    Rational timebase;
    Rational framerate;

    int gopSize;
    int maxBFrames;

    PIXEL_FORMAT pixelFormat;
};

};