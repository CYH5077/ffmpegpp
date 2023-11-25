#pragma once

#include <iostream>
#include <memory>

#include "AVType.hpp"
#include "AVResult.hpp"


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

public: // getter setter
    int64_t getPTS();
    int64_t getDTS();
    int     getSize();
    int     getStreamIndex();

    void setPos(int pos);
    void setStreamIndex(int streamIndex);
    
public: // Raw pointer
    AVPacket* getRawPacket();

private:
    AVPacket* packet;
};


};