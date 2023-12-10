#pragma once

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Packet.hpp"
#include "Rational.hpp"


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
    Rational getTimebase();
    Rational getFrameRate();
    int getWidth();
    int getHeight();

    int getVideoAVCodecID();
    int getAudioAVCodecID();

    int getVideoStreamIndex();
    int getAudioStreamIndex();
    
    unsigned int getStreamCount();

    bool isVideoCodecParameters();
    bool isAudioCodecParameters();
    
    
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

    AVStream* videoStream;
    AVStream* audioStream;
    
    AVCodecParameters* videoCodecParameter;
    AVCodecParameters* audioCodecParameter;
};


};