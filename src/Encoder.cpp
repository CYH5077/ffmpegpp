#include "Encoder.hpp"
#include "Rational.hpp"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
}

namespace av {

Encoder::Encoder(CodecContextPtr videoContext, CodecContextPtr audioContext) {
    this->videoContext = videoContext;
    this->audioContext = audioContext;
}

Encoder::~Encoder() {

}

bool Encoder::encode(Muxer& muxer, MEDIA_TYPE type, Frame& frame, AVResult* result) {
    switch(type) {
    case MEDIA_TYPE::VIDEO: return this->encodeFrame(muxer, this->videoContext, frame, result);
    case MEDIA_TYPE::AUDIO: return this->encodeFrame(muxer, this->audioContext, frame, result);
    default:
        return result->failed(-1, "Not support MEIDA_TYPE");
    }
};

bool Encoder::encodeFrame(Muxer& muxer, CodecContextPtr codecContext, Frame& frame, AVResult* result) {
    if (codecContext == nullptr) {
        return true;
    }
    
    Packet packet;
    int ret = avcodec_send_frame(codecContext->getRawCodecContext(), frame.getRawFrame());
    while (ret >= 0) {
        ret = avcodec_receive_packet(codecContext->getRawCodecContext(), packet.getRawPacket());
        if (ret == AVERROR(EAGAIN) || 
            ret == AVERROR_EOF) {
            break;
        }

        AVMediaType mediaType = codecContext->getRawCodecContext()->codec_type;
        AVStream* muxerStream = muxer.getRawStream(av::AVMediaTypeToMediaType(mediaType));
        if (muxerStream == nullptr) {
            return result->failed(-1, "Not found stream info");
        }

        Rational frameTimeBase = frame.getTimeBase();
        av_packet_rescale_ts(packet.getRawPacket(), AVRational{frameTimeBase.getNum(), frameTimeBase.getDen()}, muxerStream->time_base);
        if (!muxer.writePacket(packet, result)) {
            return result->isSuccess();
        }

        packet.unref();
    }
    return result->success();
}

};
