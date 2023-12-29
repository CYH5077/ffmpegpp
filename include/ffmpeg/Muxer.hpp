#pragma once

#include <string>
#include <vector>

#include "ffmpeg/AVType.hpp"
#include "ffmpeg/AVResult.hpp"
#include "ffmpeg/Demuxer.hpp"
#include "ffmpeg/CodecContext.hpp"
#include "ffmpeg/Packet.hpp"
#include "ffmpeg/Stream.hpp"

namespace av {
    class Muxer {
    public:
        explicit Muxer();
        virtual ~Muxer();

    public:
        Muxer(const Muxer&) = delete;
        Muxer& operator=(const Muxer&) = delete;

    public:
        bool transMux(Demuxer& demuxer, const std::string& saveFileName, AVResult* result);

        bool open(const std::string& fileName, AVResult* result);
        void close();

        bool copyStreamsFrom(Demuxer& demuxer, AVResult* result);
        bool createNewStream(CodecContextPtr codecContext, AVResult* result);

        bool writeHeader(AVResult* result);
        bool writePacket(Packet& packet, AVResult* result);

    public: // getter setter
        const Stream& getVideoStream();
        const Stream& getAudioStream();

    public: // Raw pointer
        AVFormatContext* getRawFormatContext();
        AVStream* getRawStream(MEDIA_TYPE type);

    private:
        bool copyPacketsFrom(Demuxer& demuxer, AVResult* result);

        void clear();

    private:
        AVFormatContext* formatContext;

        Stream videoStream;
        Stream audioStream;

        std::vector<int> streamsMapper;
    };
};