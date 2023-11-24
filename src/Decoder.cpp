#include "Decoder.hpp"

namespace av {

Decoder::Decoder(CodecContext& videoContext, CodecContext& audioContext) 
: videoContext(videoContext)
, audioContext(audioContext) {

}

Decoder::~Decoder() {

}

bool Decoder::decode(Demuxer& demuxer, std::function<void(AVMediaType, Frame&)> func, AVResult* result) {
    if (result == nullptr) {
        return false;
    }
    this->func = func;

    Frame frame;
    Packet packet;
    while (demuxer.read(&packet, result)) {
        bool decodeResult;
        if (packet.getStreamIndex() == demuxer.getVideoStreamIndex()) {
            decodeResult = this->decodePacket(this->videoContext, packet, &frame, result);
        } else if (packet.getStreamIndex() == demuxer.getAudioStreamIndex()) {
            decodeResult = this->decodePacket(this->audioContext, packet, &frame, result);
        }
        packet.unref();

        if (decodeResult == false) {
            break;
        }
    }

    return result->success();
}

bool Decoder::decodePacket(CodecContext& codecContext, Packet& packet, Frame* frame, AVResult* result) {
    int ret = 0;

    ret = avcodec_send_packet(codecContext.getRawCodecContext(), packet.getRawPacket());
    if (ret < 0) {
        char avErrorMessage[AV_ERROR_MAX_STRING_SIZE] = {0, };
        av_strerror(ret, avErrorMessage, sizeof(avErrorMessage));
        std::cout << "avcodec_send_packet() failed: " << avErrorMessage << std::endl;
        return result->avFailed(ret);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(codecContext.getRawCodecContext(), frame->getRawFrame());
        if (ret < 0) {
            if (ret == AVERROR_EOF || 
                ret == AVERROR(EAGAIN)) {
                return result->success();
            }
            return result->avFailed(ret);
        }

        this->func(codecContext.getRawCodecContext()->codec->type, *frame);
        
        frame->unref();
    }

    return result->success();
}

};