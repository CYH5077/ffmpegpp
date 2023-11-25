#include "Encoder.hpp"

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

bool Encoder::encode(MEDIA_TYPE type, Muxer& muxer, Frame& frame, AVResult* result) {
    switch(type) {
    case MEDIA_TYPE::VIDEO: return this->encodeFrame(this->videoContext, muxer, frame, result);
    case MEDIA_TYPE::AUDIO: return this->encodeFrame(this->audioContext, muxer, frame, result);
    default:
        return result->failed(-1, "Not support MEIDA_TYPE");
    }
};

bool Encoder::encodeFrame(CodecContextPtr codecContext, Muxer& muxer, Frame& frame, AVResult* result) {
    if (codecContext == nullptr) {
        return true;
    }
    
    Packet packet;
    int ret = avcodec_send_frame(codecContext->getRawCodecContext(), frame.getRawFrame());
    while (ret >= 0) {
        ret = avcodec_receive_packet(this->videoContext->getRawCodecContext(), packet.getRawPacket());
        if (ret == AVERROR(EAGAIN) || 
            ret == AVERROR_EOF) {
            break;
        }

        if (!muxer.writePacket(packet, result)) {
            return result->isSuccess();
        }

        packet.unref();
    }
    return result->success();
}

};
