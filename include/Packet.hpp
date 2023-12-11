#pragma once

#include <iostream>
#include <memory>

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Rational.hpp"


namespace av {

class Packet {
public:
    explicit Packet();
    explicit Packet(AVPacket* packet);
    virtual ~Packet();

public:
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

public:
    void unref();

    void rescaleTS(const Rational&& preTimebase, const Rational&& targetTimebase);

public: // getter setter
    bool isValidPacket();

    int64_t getPTS();
    int64_t getDTS();
    int     getSize();
    int     getStreamIndex();
    MEDIA_TYPE getMediaType();
    double  getPTSTimeToSecond(const Rational&& timebase);

    void setPTS(int64_t pts);
    void setDTS(int64_t dts);
    void setPos(int pos);
    void setStreamIndex(int streamIndex);
    void setMediaType(MEDIA_TYPE mediaType);
    
public: // Raw pointer
    AVPacket* getRawPacket();

    void setRawPacket(AVPacket* packet);
    
private:
    MEDIA_TYPE mediaType;

    AVPacket* packet;
};


};