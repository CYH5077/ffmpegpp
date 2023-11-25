#include "Packet.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

Packet::Packet() {
    this->packet = av_packet_alloc();
}

Packet::Packet(AVPacket* packet) {
    this->packet = packet;
}

Packet::~Packet() {
    if (this->packet != nullptr) {
        av_packet_free(&this->packet);
    }
}

void Packet::unref() {
    if (this->packet == nullptr) {
        return;
    }

    av_packet_unref(this->packet);
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

void Packet::setPos(int pos) {
    this->packet->pos = pos;
}

void Packet::setStreamIndex(int streamIndex) {
    this->packet->stream_index = streamIndex;
}

AVPacket* Packet::getRawPacket() {
    return this->packet;
}

};