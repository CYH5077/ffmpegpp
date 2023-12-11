#pragma once

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Packet.hpp"
#include "Rational.hpp"
#include "Stream.hpp"

namespace av {


class Demuxer {
public:
    explicit Demuxer();
    virtual ~Demuxer();

public:
    Demuxer(const Demuxer&) = delete;
    Demuxer& operator=(const Demuxer&) = delete;

public:
    bool open(const std::string& fileName, AVResult* result);
    void close();

    bool read(Packet* packet, AVResult* result);

    void printDump();

public: // getter setter
    const Stream& getVideoStream();
    const Stream& getAudioStream();
    
    int getWidth();
    int getHeight();

    int getVideoAVCodecID();
    int getAudioAVCodecID();

    int getVideoStreamIndex();
    int getAudioStreamIndex();
    
    unsigned int getStreamCount();

    bool isValidVideoCodecParameters();
    bool isValidAudioCodecParameters();
    
    
public: // Raw Pointer
    AVFormatContext* getRawFormatContext();

    AVCodecParameters* getRawVideoCodecParameters();
    AVCodecParameters* getRawAudioCodecParameters();

    AVStream* getRawStream(int index);
    AVStream* getRawVideoStream();
    AVStream* getRawAudioStream();

private:
    bool createFormatContext(AVResult* result);
    bool openFormatContext(const std::string& fileName, AVResult* result);
    
    void findCodecParameters();
    int  findBestStream(MEDIA_TYPE type);

    bool readPacket(Packet* packet, AVResult* result);

    void clear();

private:
    AVFormatContext* formatContext;

    int videoStreamIndex;
    int audioStreamIndex;

    Stream videoStream;
    Stream audioStream;
    
    AVCodecParameters* videoCodecParameter;
    AVCodecParameters* audioCodecParameter;
};


};