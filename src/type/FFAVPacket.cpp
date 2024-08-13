#include "type/FFAVPacket.hpp"

#include "type/FFAVDecodeStream.hpp"
#include "type/FFAVEncodeStream.hpp"
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

    int FFAVPacket::getFrameNumber() {
        return this->frameNumber;
    }

    void FFAVPacket::setStreamIndex(int streamIndex) {
        this->packetImpl->getRaw()->stream_index = streamIndex;
    }

    void FFAVPacket::setFrameNumber(int frameNumber) {
        this->frameNumber = frameNumber;
    }

    void FFAVPacket::rescaleTS(FFAVDecodeStreamPtr srcStream, FFAVEncodeStreamPtr dstStream) {
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

    // FFAVPacketList
    FFAVPacketList::~FFAVPacketList() {
        for (auto& packet : *this) {
            av_packet_unref(packet.getImpl()->getRaw().get());
        }
    }
}
