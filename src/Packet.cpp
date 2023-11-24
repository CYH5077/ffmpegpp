#include "Packet.hpp"

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

AVPacket* Packet::getRawPacket() {
    return this->packet;
}

};