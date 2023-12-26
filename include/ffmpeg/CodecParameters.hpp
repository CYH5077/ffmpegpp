#pragma once

#include "AVType.hpp"
#include "AVResult.hpp"
#include "ChannelLayout.hpp"

namespace av {
    class CodecParameters {
    public:
        explicit CodecParameters();
        explicit CodecParameters(AVCodecParameters* codecParameters);
        virtual ~CodecParameters();

    public: // getter setter
        bool isValidCodecParameters() const;

        long long getBitrate() const;
        int getCodecID() const;
        int getWidth() const;
        int getHeight() const;
        int getSamplerate() const;

        const ChannelLayout& getChannelLayout() const;

    public: // Raw pointer
        void setRawCodecParameters(AVCodecParameters* codecParameters);

        AVCodecParameters* getRawCodecParameters() const;

    private:
        AVCodecParameters* codecParameters;

        ChannelLayout channelLayout;
    };
};