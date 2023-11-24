#pragma once

#include <iostream>
#include <memory>

extern "C" {
    #include "libavformat/avformat.h"
}

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

public: // getter 
    int64_t getPTS();
    int64_t getDTS();
    int     getSize();

public: // Raw pointer
    AVPacket* getRawPacket();

private:
    AVPacket* packet;
};


};