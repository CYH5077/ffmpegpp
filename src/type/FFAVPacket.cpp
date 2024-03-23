#include "type/FFAVPacket.hpp"
#include "type/impl/FFAVPacketImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
	// FFAVPacket 
	FFAVPacket::FFAVPacket() {
		this->packetImpl = FFAVPacketImpl ::create();
	}

	FFAVPacket::FFAVPacket(FFAVPacket& packet) {
		this->packetImpl = FFAVPacketImpl::create(packet.packetImpl);
	}

	FFAVPacket::FFAVPacket(FFAVPacketImplPtr packet) {
		this->packetImpl = FFAVPacketImpl::create(packet);
	}

	FFAVPacket::~FFAVPacket() {

	}

	FFAVPacketImplPtr FFAVPacket::getImpl() {
		return this->packetImpl;
	}
}