#pragma once

#include "AVResult.hpp"
#include "Packet.hpp"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
}

namespace av {


class Demuxer {
public:
    explicit Demuxer();
    virtual ~Demuxer();

public:
    bool open(const std::string& fileName, AVResult* result);
    void close();

    bool read(Packet* packet, AVResult* result);

    void printDump();

public: // getter setter
    AVCodecID getVideoCodecID();
    AVCodecID getAudioCodecID();

    int getVideoStreamIndex();
    int getAudioStreamIndex();
    
    bool isVideoCodecParameters();
    bool isAudioCodecParameters();
    
public: // Raw Pointer
    AVFormatContext* getRawFormatContext();

    AVCodecParameters* getRawVideoCodecParameters();
    AVCodecParameters* getRawAudioCodecParameters();


private:
    bool createFormatContext(AVResult* result);
    bool openFormatContext(const std::string& fileName, AVResult* result);
    
    void findCodecParameters();
    int  findBestStream(enum AVMediaType type);

    bool readPacket(Packet* packet, AVResult* result);

private:
    AVFormatContext* formatContext;

    int videoStreamIndex;
    int audioStreamIndex;

    AVCodecParameters* videoCodecParameter;
    AVCodecParameters* audioCodecParameter;
};


};