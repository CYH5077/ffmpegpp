#include "Demuxer.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

Demuxer::Demuxer() {
    this->clear();
}

Demuxer::~Demuxer() {
    this->close();
}

bool Demuxer::open(const std::string& fileName, AVResult* result) {
    if (result == nullptr) {
        return false;
    }

    if (!this->createFormatContext(result)) {
        return result->isSuccess();
    }

    if (!this->openFormatContext(fileName, result)) {
        return result->isSuccess();
    }

    this->findCodecParameters();

    return result->success();
}

void Demuxer::close() {
    if (this->formatContext != nullptr) {
        avformat_close_input(&this->formatContext);
    }
    this->clear();
}

bool Demuxer::read(Packet* packet, AVResult* result) {
    if (packet == nullptr ||
        result == nullptr) {
        return false;
    }

    return this->readPacket(packet, result);
}

void Demuxer::printDump() {
    if (this->formatContext == nullptr) {
        return;
    }

    av_dump_format(this->formatContext, 0, nullptr, 0);
}

const Stream& Demuxer::getVideoStream() {
    return this->videoStream;
}

const Stream& Demuxer::getAudioStream() {
    return this->audioStream;
}

int Demuxer::getWidth() {
    return this->videoCodecParameter->width;
}

int Demuxer::getHeight() {
    return this->videoCodecParameter->height;
}

int Demuxer::getVideoAVCodecID() {
    if (this->videoCodecParameter == nullptr) {
        return AV_CODEC_ID_NONE;
    }
    return this->videoCodecParameter->codec_id;
}

int Demuxer::getAudioAVCodecID() {
    if (this->audioCodecParameter == nullptr) {
        return AV_CODEC_ID_NONE;
    }
    return this->audioCodecParameter->codec_id;
}

int Demuxer::getVideoStreamIndex() {
    return this->videoStreamIndex;
}

int Demuxer::getAudioStreamIndex() {
    return this->audioStreamIndex;
}

unsigned int Demuxer::getStreamCount() {
    return this->formatContext->nb_streams;
}

bool Demuxer::isValidVideoCodecParameters() {
    if (this->videoCodecParameter == nullptr) {
        return false;
    }
    return true;
}

bool Demuxer::isValidAudioCodecParameters() {
    if (this->audioCodecParameter == nullptr) {
        return false;
    }
    return true;
}

AVFormatContext* Demuxer::getRawFormatContext() {
    return this->formatContext;
}

AVCodecParameters* Demuxer::getRawVideoCodecParameters() {
    return this->videoCodecParameter;
}

AVCodecParameters* Demuxer::getRawAudioCodecParameters() {
    return this->audioCodecParameter;
}

AVStream* Demuxer::getRawStream(int index) {
    return this->formatContext->streams[index];
}

bool Demuxer::createFormatContext(AVResult* result) {
    if (this->formatContext != nullptr) {
        this->close();
    }

    this->formatContext = avformat_alloc_context();
    if (this->formatContext == nullptr) {
        return result->avFailed(AVERROR(ENOMEM));
    }

    return result->success();
}

bool Demuxer::openFormatContext(const std::string& fileName, AVResult* result) {
    int ret = avformat_open_input(&this->formatContext, fileName.c_str(), nullptr, nullptr);
    if (ret != 0) {
        return result->avFailed(ret);
    }

    ret = avformat_find_stream_info(this->formatContext, nullptr);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    return result->success();
}

void Demuxer::findCodecParameters() {
    this->videoStreamIndex = this->findBestStream(MEDIA_TYPE::VIDEO);
    this->audioStreamIndex = this->findBestStream(MEDIA_TYPE::AUDIO);

    if (this->videoStreamIndex >= 0) {
        this->videoStream = Stream(this->formatContext->streams[this->videoStreamIndex]);
        this->videoCodecParameter = this->videoStream.getRawCodecParameters();
    }

    if (this->audioStreamIndex >= 0) {
        this->audioStream = Stream(this->formatContext->streams[this->audioStreamIndex]);
        this->audioCodecParameter = this->audioStream.getRawCodecParameters();
    }
}

int Demuxer::findBestStream(MEDIA_TYPE type) {
    return av_find_best_stream(this->formatContext, (AVMediaType)av::mediaTypeToAVMediaType(type), -1, -1, nullptr, 0);
}

bool Demuxer::readPacket(Packet* packet, AVResult* result) {
    int avResult = av_read_frame(this->formatContext, packet->getRawPacket());
    if (avResult < 0) {
        return result->failed(avResult, "File EOF");
    }

    if (packet->getStreamIndex() == this->getVideoStreamIndex()) {
        packet->setMediaType(MEDIA_TYPE::VIDEO);
    } else if (packet->getStreamIndex() == this->getAudioStreamIndex()) {
        packet->setMediaType(MEDIA_TYPE::AUDIO);
    } 

    return result->success();
}

void Demuxer::clear() {
    this->formatContext = nullptr;

    this->videoStreamIndex = -1;
    this->audioStreamIndex = -1;

    this->videoCodecParameter = nullptr;
    this->audioCodecParameter = nullptr;
}

};
