#pragma once

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Packet.hpp"
#include "Rational.hpp"
#include "Stream.hpp"
#include "CodecParameters.hpp"
#include "VideoEncodeParameters.hpp"
#include "AudioEncodeParameters.hpp"
#include "VideoStreamInfo.hpp"
#include "AudioStreamInfo.hpp"

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
        bool seekToFirstFrame(AVResult* result);
        bool seekVideo(double seconds, AVResult* result);
        bool seekAudio(double seconds, AVResult* result);

    public: // getter setter
        int getPlayTimeToSeconds();

        VideoStreamInfoPtr getVideoStreamInfo();
        AudioStreamInfoPtr getAudioStreamInfo();

        const Stream& getVideoStream();
        const Stream& getAudioStream();

        const CodecParameters& getVideoCodecParameters();
        const CodecParameters& getAudioCodecParameters();

        int getVideoStreamIndex();
        int getAudioStreamIndex();

        // 내부에서 Packet을 전부 읽어들이고 다시 AVFormatContext의 인덱스를 처음으로 되돌림.
        int getPacketCount(AVResult* result);

        unsigned int getStreamCount();

    public: // Raw Pointer
        AVFormatContext* getRawFormatContext();

        AVStream* getRawStream(int index);

    private:
        bool createFormatContext(AVResult* result);
        bool openFormatContext(const std::string& fileName, AVResult* result);

        void findCodecParameters();
        int  findBestStream(MEDIA_TYPE type);

        bool readPacket(Packet* packet, AVResult* result);

        bool seek(int streamIndex,  long long timestamp, AVResult* result);
        void clear();

    private:
        AVFormatContext* formatContext;

        int videoStreamIndex;
        int audioStreamIndex;

        Stream videoStream;
        Stream audioStream;

        CodecParameters videoCodecParameter;
        CodecParameters audioCodecParameter;
    };
};