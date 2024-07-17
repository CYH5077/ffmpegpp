#pragma once

#include <memory>

#include "type/ffav.hpp"

namespace ff {
    class FFAVVideoEncodeParameters;
    typedef std::shared_ptr<FFAVVideoEncodeParameters> FFAVVideoEncodeParametersPtr;

    class FFAVVideoEncodeParameters {
    public:
        explicit FFAVVideoEncodeParameters();
        explicit FFAVVideoEncodeParameters(FFAVInputContext& inputContext);
        explicit FFAVVideoEncodeParameters(FFAVVideoEncodeParametersPtr encodeParameters);

        virtual ~FFAVVideoEncodeParameters();

    public:
        static FFAVVideoEncodeParametersPtr create();
        static FFAVVideoEncodeParametersPtr create(FFAVInputContext& inputContext);
        static FFAVVideoEncodeParametersPtr create(FFAVVideoEncodeParametersPtr encodeParameters);

    public:
        void copyFrom(FFAVInputContext& inputContext);

        long long getBitrate() const;
        int getWidth() const;
        int getHeight() const;
        int getGOPSize() const;
        int getMaxBFrames() const;
        const FFAVRational& getTimeBase() const;
        int getPixelFormat() const;
        int getEncodeThreadCount() const;

        void setBitrate(long long bitrate);
        void setWidth(int width);
        void setHeight(int height);
        void setGOPSize(int gopSize);
        void setMaxBFrames(int maxBFrames);
        void setTimeBase(const FFAVRational& timebase);
        void setPixelFormat(int pixelFormat);
        void setEncodeThreadCount(int threadCount);

    private:
        long long bitrate;

        int width;
        int height;

        int gopSize;
        int maxBFrames;

        FFAVRational timebase;

        int pixelFormat;

        int encodeThreadCount;
    };
};
