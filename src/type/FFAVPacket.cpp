#include "type/FFAVPacket.hpp"

#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    // FFAVPacket
    FFAVPacket::FFAVPacket() {
        this->packetImpl = FFAVPacketImpl ::create();
    }

    FFAVPacket::~FFAVPacket() {}


    FFAVPacketImplPtr FFAVPacket::getImpl() {
        return this->packetImpl;
    }

    int FFAVPacket::getStreamIndex() {
		return this->packetImpl->getRaw()->stream_index;
	}

    void FFAVPacket::setStreamIndex(int streamIndex) {
		this->packetImpl->getRaw()->stream_index = streamIndex;
	}

}
