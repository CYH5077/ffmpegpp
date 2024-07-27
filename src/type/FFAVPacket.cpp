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
        this->init();
    }

    FFAVPacket::FFAVPacket(FFAVPacket& packet) {
        this->packetImpl = FFAVPacketImpl::create(packet.packetImpl);
        this->init();
    }

    FFAVPacket::FFAVPacket(FFAVPacketImplPtr packet) {
        this->packetImpl = FFAVPacketImpl::create(packet);
        this->init();
    }

    FFAVPacket::~FFAVPacket() {}

    void FFAVPacket::rescaleTS(ff::FFAVStreamPtr srcStream, ff::FFAVStreamPtr dstStream) {
        AVRational srcTimebase = srcStream->getImpl()->getRaw()->time_base;
        AVRational dstTimebase = dstStream->getImpl()->getRaw()->time_base;

        AVPacket* packetRaw = this->packetImpl->getRaw().get();
        if (packetRaw->pts != AV_NOPTS_VALUE) {
            av_packet_rescale_ts(packetRaw, srcTimebase, dstTimebase);
        } else {
            if (packetRaw->dts != AV_NOPTS_VALUE) {
                packetRaw->pts = packetRaw->dts;
            } else {
                packetRaw->dts = packetRaw->pts = av_rescale_q(this->frameNumber, srcTimebase, dstTimebase);
            }
        }
    }
    void FFAVPacket::setType(ff::DATA_TYPE type) {
        this->type = type;
    }

    void FFAVPacket::setFrameNumber(int frameNumber) {
        this->frameNumber = frameNumber;
    }

    void FFAVPacket::setStreamIndex(int streamIndex) {
        AVPacket* packet = this->packetImpl->getRaw().get();
        packet->stream_index = streamIndex;
    }

    void FFAVPacket::setDecodeStream(FFAVStreamPtr decodeStream) {
        this->decodeStream = decodeStream;
    }

    void FFAVPacket::setEncodeStream(FFAVStreamPtr encodeStream) {
		this->encodeStream = encodeStream;
	}

    DATA_TYPE FFAVPacket::getType() {
        return this->type;
    }

    int FFAVPacket::getFrameNumber() {
        return this->frameNumber;
    }

    FFAVPacketImplPtr FFAVPacket::getImpl() {
        return this->packetImpl;
    }

    FFAVStreamPtr FFAVPacket::getDecodeStream() {
        return this->decodeStream;
    }

    FFAVStreamPtr FFAVPacket::getEncodeStream() {
        return this->encodeStream;
    }

    int FFAVPacket::getStreamIndex() {
		AVPacket* packet = this->packetImpl->getRaw().get();
		return packet->stream_index;
    }

    void FFAVPacket::init() {
        this->frameNumber = 0;
        this->type = DATA_TYPE::UNKNOWN;
        this->decodeStream = nullptr;
    }
}
