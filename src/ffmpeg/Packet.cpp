#include "ffmpeg/Packet.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace av {

    Packet::Packet() {
        this->packet      = av_packet_alloc();
        this->mediaType   = MEDIA_TYPE::UNKNOWN;
        this->frameNumber = 0;
    }

    Packet::Packet(AVPacket* packet, MEDIA_TYPE mediaType) {
        this->packet = av_packet_alloc();
        if (packet != nullptr) {
            av_packet_ref(this->packet, packet);
        }
        this->mediaType   = mediaType;
        this->frameNumber = 0;
    }

    Packet::~Packet() {
        if (this->isValidPacket() == true) {
            av_packet_free(&this->packet);
        }
    }

    void Packet::unref() {
        if (this->isValidPacket() == false) {
            return;
        }

        av_packet_unref(this->packet);
    }

    void Packet::rescaleTS(const Rational& preTimebase, const Rational& targetTimebase) {
        AVRational avPreTimebase    { preTimebase.getNum()   , preTimebase.getDen() };
        AVRational avTargetTimebase { targetTimebase.getNum(), targetTimebase.getDen() };

        if (this->packet->pts != AV_NOPTS_VALUE) {
            av_packet_rescale_ts(this->packet, avPreTimebase, avTargetTimebase);
        } else {
            if (this->packet->dts != AV_NOPTS_VALUE) {
                this->packet->pts = packet->dts;
            } else {
                this->packet->dts = this->packet->pts = av_rescale_q(this->frameNumber, avPreTimebase, avTargetTimebase);
            }
        }
    }

    bool Packet::isValidPacket() {
        if (this->packet == nullptr) {
            return false;
        }
        return true;
    }

    int64_t Packet::getPTS() {
        return this->packet->pts;
    }

    int64_t Packet::getDTS() {
        return this->packet->dts;
    }

    int Packet::getSize() {
        return this->packet->size;
    }

    int Packet::getStreamIndex() {
        return this->packet->stream_index;
    }

    MEDIA_TYPE Packet::getMediaType() {
        return this->mediaType;
    }

    long long Packet::getFrameNumber() {
        return this->frameNumber;
    }

    double Packet::getPTSTimeToSecond(const Rational&& timebase) {
        AVRational avTimebase {timebase.getNum(), timebase.getDen()};
        double ptsTime = this->getPTS() * av_q2d(avTimebase);
        return ptsTime;
    }

    void Packet::setPTS(int64_t pts) {
        this->packet->pts = pts;
    }

    void Packet::setDTS(int64_t dts) {
        this->packet->dts = dts;
    }

    void Packet::setPos(int pos) {
        this->packet->pos = pos;
    }

    void Packet::setStreamIndex(int streamIndex) {
        this->packet->stream_index = streamIndex;
    }

    void Packet::setMediaType(MEDIA_TYPE mediaType) {
        this->mediaType = mediaType;
    }

    void Packet::setFrameNumber(long long frameNumber) {
        this->frameNumber = frameNumber;
    }

    AVPacket* Packet::getRawPacket() {
        return this->packet;
    }

    void Packet::setRawPacket(AVPacket* packet) {
        this->packet = packet;
    }

};