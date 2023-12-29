#pragma once

#include "ffmpeg/Stream.hpp"
#include "ffmpeg/CodecParameters.hpp"

#include <memory>

namespace av {
    class VideoStreamInfo {
    public:
        static std::shared_ptr<VideoStreamInfo> createVideoStreamInfo(Stream& stream, CodecParameters& codecParameters);

    public:
        explicit VideoStreamInfo(Stream& stream, CodecParameters& codecParameters);
        virtual ~VideoStreamInfo();

    public: // getter setter
        long long getBitrate();
        int       getWidth();
        int       getHeight();

        const Rational& getTimebase();
        const Rational& getFramerate();

    private:
        Stream stream;
        CodecParameters codecParameters;
    };

    typedef std::shared_ptr<VideoStreamInfo> VideoStreamInfoPtr;
};