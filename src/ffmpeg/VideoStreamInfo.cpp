#include "ffmpeg/VideoStreamInfo.hpp"

#include <memory>

namespace av {
    std::shared_ptr<VideoStreamInfo> VideoStreamInfo::createVideoStreamInfo(av::Stream& stream, av::CodecParameters& codecParameters) {
        std::shared_ptr<VideoStreamInfo> streamInfo = nullptr;
        if (stream.isValidStream()                   == true &&
            codecParameters.isValidCodecParameters() == true) {
            streamInfo = std::make_shared<VideoStreamInfo>(stream, codecParameters);
        }

        return streamInfo;
    }

    VideoStreamInfo::VideoStreamInfo(av::Stream &stream, av::CodecParameters &codecParameters) {
        this->stream = stream;
        this->codecParameters = codecParameters;
    }

    VideoStreamInfo::~VideoStreamInfo() {

    }

    long long VideoStreamInfo::getBitrate() {
        return this->codecParameters.getBitrate();
    }

    int VideoStreamInfo::getWidth() {
        return this->codecParameters.getWidth();
    }

    int VideoStreamInfo::getHeight() {
        return this->codecParameters.getHeight();
    }

    const Rational& VideoStreamInfo::getTimebase() {
        return this->stream.getTimebase();
    }

    const Rational& VideoStreamInfo::getFramerate() {
        return this->stream.getFramerate();
    }

};