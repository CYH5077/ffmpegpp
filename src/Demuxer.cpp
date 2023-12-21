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

    bool Demuxer::seekToFirstFrame(AVResult* result) {
        return this->seek(-1, 0, result);
    }

    bool Demuxer::seekVideo(double seconds, AVResult* result) {
        Rational timebase = this->videoStream.getTimebase();
        int64_t timestamp = seconds / av_q2d(AVRational {timebase.getNum(), timebase.getDen()});
        return this->seek(this->videoStreamIndex, timestamp, result);
    }

    bool Demuxer::seekAudio(double seconds, av::AVResult *result) {
        Rational timebase = this->audioStream.getTimebase();
        int64_t timestamp = seconds / av_q2d(AVRational {timebase.getNum(), timebase.getDen()});
        return this->seek(this->audioStreamIndex, timestamp, result);
    }

    const Stream& Demuxer::getVideoStream() {
        return this->videoStream;
    }

    const Stream& Demuxer::getAudioStream() {
        return this->audioStream;
    }

    const CodecParameters& Demuxer::getVideoCodecParameters() {
        return this->videoCodecParameter;
    }

    const CodecParameters& Demuxer::getAudioCodecParameters() {
        return this->audioCodecParameter;
    }

    int Demuxer::getVideoStreamIndex() {
        return this->videoStreamIndex;
    }

    int Demuxer::getAudioStreamIndex() {
        return this->audioStreamIndex;
    }

    int Demuxer::getPacketCount(AVResult* result) {
        int ret = 0;

        Packet packet;
        while (this->read(&packet, result)) {
            ret++;
        }

        if (result->isSuccess() == false &&
            result->isFileEOF() == false) {
            return -1;
        }

        // 다시 시작 지점으로 되돌림
        if (this->seekToFirstFrame(result) == false) {
            return result->isSuccess();
        }

        return ret;
    }

    unsigned int Demuxer::getStreamCount() {
        return this->formatContext->nb_streams;
    }

    AVFormatContext* Demuxer::getRawFormatContext() {
        return this->formatContext;
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
            this->videoCodecParameter.setRawCodecParameters(this->videoStream.getRawCodecParameters());
        }

        if (this->audioStreamIndex >= 0) {
            this->audioStream = Stream(this->formatContext->streams[this->audioStreamIndex]);
            this->audioCodecParameter.setRawCodecParameters(this->audioStream.getRawCodecParameters());
        }
    }

    int Demuxer::findBestStream(MEDIA_TYPE type) {
        return av_find_best_stream(this->formatContext, (AVMediaType)av::mediaTypeToAVMediaType(type), -1, -1, nullptr, 0);
    }

    bool Demuxer::readPacket(Packet* packet, AVResult* result) {
        int avResult = av_read_frame(this->formatContext, packet->getRawPacket());
        if (avResult < 0) {
            if (avResult == AVERROR_EOF || avio_feof(this->formatContext->pb)) {
                return result->avFailedFileEOF();
            }
            return result->failed(avResult, "File EOF");
        }

        if (packet->getStreamIndex() == this->getVideoStreamIndex()) {
            packet->setMediaType(MEDIA_TYPE::VIDEO);
        } else if (packet->getStreamIndex() == this->getAudioStreamIndex()) {
            packet->setMediaType(MEDIA_TYPE::AUDIO);
        }

        return result->success();
    }

    bool Demuxer::seek(int streamIndex, long long timestamp, av::AVResult *result) {
        int ret = av_seek_frame(this->formatContext, streamIndex, timestamp, AVSEEK_FLAG_BACKWARD);
        if (ret < 0) {
            return result->avFailed(ret);
        }

        return result->success();
    }

    void Demuxer::clear() {
        this->formatContext = nullptr;

        this->videoStreamIndex = -1;
        this->audioStreamIndex = -1;
    }

};
