#pragma once

#include "ffmpeg/Stream.hpp"
#include "ffmpeg/CodecParameters.hpp"
#include "ffmpeg/Rational.hpp"
#include "ffmpeg/ChannelLayout.hpp"

#include <memory>

namespace av {
    class AudioStreamInfo {
    public:
        static std::shared_ptr<AudioStreamInfo> createAudioStreamInfo(Stream& stream, CodecParameters& codecParameters);

    public:
        explicit AudioStreamInfo(Stream& stream, CodecParameters& codecParameters);
        virtual ~AudioStreamInfo();

    public: // getter setter
        long long getBitrate();
        int       getSamplerate();

        const Rational&      getTimebase();
        const ChannelLayout& getChannelLayout();

    private:
        Stream stream;
        CodecParameters codecParameters;
    };

    typedef std::shared_ptr<AudioStreamInfo> AudioStreamInfoPtr;
};