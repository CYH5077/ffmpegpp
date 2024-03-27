#include "type/FFAVPacket.hpp"

#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    DATA_TYPE DATA_TYPE_FROM_AV_CODEC_TYPE(int codecType) {
        switch (codecType) {
        case AVMEDIA_TYPE_VIDEO:
            return DATA_TYPE::VIDEO;
        case AVMEDIA_TYPE_AUDIO:
            return DATA_TYPE::AUDIO;
        case AVMEDIA_TYPE_SUBTITLE:
            return DATA_TYPE::SUBTITLE;
        case AVMEDIA_TYPE_DATA:
            return DATA_TYPE::DATA;
        default:
            return DATA_TYPE::UNKNOWN;
        }
    }

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

    void FFAVPacket::rescaleTS(ff::FFAVStreamPtr srcStream, ff::FFAVStreamPtr dstStream) {
        AVRational srcTimebase = srcStream.get()->getImpl()->getRaw()->time_base;
        AVRational dstTimebase = dstStream.get()->getImpl()->getRaw()->time_base;

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

    DATA_TYPE FFAVPacket::getType() {
        return this->type;
    }

    int FFAVPacket::getFrameNumber() {
        return this->frameNumber;
    }

	FFAVPacketImplPtr FFAVPacket::getImpl() {
		return this->packetImpl;
	}
}