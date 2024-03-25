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

	FFAVPacketImplPtr FFAVPacketImpl::create(FFAVPacketImplPtr packetImpl) {
		return std::make_shared<FFAVPacketImpl>(packetImpl);
	}



	// FFAVPacketImpl
	FFAVPacketImpl::FFAVPacketImpl() {
		this->packet = std::shared_ptr<AVPacket>(newAVPacket(), freeAVPacket);
	}

	FFAVPacketImpl::FFAVPacketImpl(FFAVPacketImplPtr packetImpl) {
        this->copyFrom(packetImpl);
	}

	FFAVPacketImpl::~FFAVPacketImpl() {

	}
	
	AVPacketPtr FFAVPacketImpl::getRaw() {
		return this->packet;
	}

	void FFAVPacketImpl::setRaw(AVPacketPtr packet) {
		this->packet = packet;
	}

	FFAVPacketImplPtr FFAVPacketImpl::copy() {
		return FFAVPacketImpl::create(this->shared_from_this());
	}

	void FFAVPacketImpl::copyFrom(FFAVPacketImplPtr packetImpl) {
        this->copyFrom(packetImpl->getRaw());
	}

	void FFAVPacketImpl::copyFrom(AVPacketPtr packet) {
        AVPacket* rawPacket = av_packet_clone(packet.get());
        this->packet = std::shared_ptr<AVPacket>(rawPacket, freeAVPacket);
	}
};