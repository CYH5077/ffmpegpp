#include "type/impl/FFAVPacketImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

static AVPacket* newAVPacket() {
    return av_packet_alloc();
}

static void freeAVPacket(AVPacket* packet) {
    av_packet_free(&packet);
}

namespace ff {
    // Impl »ý¼º
    FFAVPacketImplPtr FFAVPacketImpl::create() {
        return std::make_shared<FFAVPacketImpl>();
    }

    FFAVPacketImpl::FFAVPacketImpl() {
        this->packet = std::shared_ptr<AVPacket>(newAVPacket(), freeAVPacket);
    }

    FFAVPacketImpl::~FFAVPacketImpl() {}

    AVPacketPtr FFAVPacketImpl::getRaw() {
        return this->packet;
    }
};
