#pragma once

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Packet.hpp"

struct AVFormatContext;
struct AVCodecParameters;
struct AVStream;

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
    int getVideoAVCodecID();
    int getAudioAVCodecID();

    int getVideoStreamIndex();
    int getAudioStreamIndex();
    
    uint getStreamCount();

    bool isVideoCodecParameters();
    bool isAudioCodecParameters();
    
    
public: // Raw Pointer
    AVFormatContext* getRawFormatContext();

    AVCodecParameters* getRawVideoCodecParameters();
    AVCodecParameters* getRawAudioCodecParameters();

    AVStream* getRawStream(int index);

private:
    bool createFormatContext(AVResult* result);
    bool openFormatContext(const std::string& fileName, AVResult* result);
    
    void findCodecParameters();
    int  findBestStream(MEDIA_TYPE type);

    bool readPacket(Packet* packet, AVResult* result);

private:
    AVFormatContext* formatContext;

    int videoStreamIndex;
    int audioStreamIndex;

    AVCodecParameters* videoCodecParameter;
    AVCodecParameters* audioCodecParameter;
};


};