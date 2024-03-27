#include "codecs/FFAVVideoEncodeParameters.hpp"

#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

namespace ff {
    FFAVVideoEncodeParametersPtr FFAVVideoEncodeParameters::create() {
        return std::make_shared<FFAVVideoEncodeParameters>();
    }

    FFAVVideoEncodeParametersPtr FFAVVideoEncodeParameters::create(FFAVInputContext& inputContext) {
        return std::make_shared<FFAVVideoEncodeParameters>(inputContext);
    }

    FFAVVideoEncodeParametersPtr FFAVVideoEncodeParameters::create(FFAVVideoEncodeParametersPtr encodeParameters) {
        return std::make_shared<FFAVVideoEncodeParameters>(encodeParameters);
    }


    FFAVVideoEncodeParameters::FFAVVideoEncodeParameters() {
        this->bitrate = 0;
        this->width = 0;
        this->height = 0;
        this->gopSize = 0;
        this->maxBFrames = 0;
        this->pixelFormat = AV_PIX_FMT_NONE;
        this->encodeThreadCount = 1;
    }

    FFAVVideoEncodeParameters::FFAVVideoEncodeParameters(FFAVInputContext& inputContext) {
        this->copyFrom(inputContext);
    }

    FFAVVideoEncodeParameters::FFAVVideoEncodeParameters(FFAVVideoEncodeParametersPtr encodeParameters) {
        *this = *encodeParameters;
    }

    FFAVVideoEncodeParameters::~FFAVVideoEncodeParameters() {

    }

    void FFAVVideoEncodeParameters::copyFrom(FFAVInputContext& inputContext) {
        AVCodecParameters* codecParameters = inputContext.getVideoCodecParameters()->getImpl()->getRaw();
        AVStream* stream                   = inputContext.getVideoStream()->getImpl()->getRaw();

        this->bitrate = codecParameters->bit_rate;
        this->width   = codecParameters->width;
        this->height  = codecParameters->height;
        this->gopSize = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
        this->maxBFrames = 0;
        this->pixelFormat = stream->codecpar->format;

        FFAVRational rational(stream->time_base.num, stream->time_base.den);
        this->timebase = rational;
    }

    long long FFAVVideoEncodeParameters::getBitrate() const {
        return this->bitrate;
    }

    int FFAVVideoEncodeParameters::getWidth() const {
        return this->width;
    }

    int FFAVVideoEncodeParameters::getHeight() const {
        return this->height;
    }

    int FFAVVideoEncodeParameters::getGOPSize() const {
        return this->gopSize;
    }

    int FFAVVideoEncodeParameters::getMaxBFrames() const {
        return this->maxBFrames;
    }

    const FFAVRational& FFAVVideoEncodeParameters::getTimeBase() const {
        return this->timebase;
    }

    int FFAVVideoEncodeParameters::getPixelFormat() const {
        return this->pixelFormat;
    }

    int FFAVVideoEncodeParameters::getEncodeThreadCount() const {
        return this->encodeThreadCount;
    }

    void FFAVVideoEncodeParameters::setBitrate(long long bitrate) {
        this->bitrate = bitrate;
    }

    void FFAVVideoEncodeParameters::setWidth(int width) {
        this->width = width;
    }

    void FFAVVideoEncodeParameters::setHeight(int height) {
        this->height = height;
    }

    void FFAVVideoEncodeParameters::setGOPSize(int gopSize) {
        this->gopSize = gopSize;
    }

    void FFAVVideoEncodeParameters::setMaxBFrames(int maxBFrames) {
        this->maxBFrames = maxBFrames;
    }

    void FFAVVideoEncodeParameters::setTimeBase(const FFAVRational& timebase) {
        this->timebase = timebase;
    }

    void FFAVVideoEncodeParameters::setPixelFormat(int pixelFormat) {
        this->pixelFormat = pixelFormat;
    }

    void FFAVVideoEncodeParameters::setEncodeThreadCount(int threadCount) {
        this->encodeThreadCount = threadCount;
    }
};