#include "Decoder.hpp"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
}

namespace av {

Decoder::Decoder(CodecContextPtr videoContext, CodecContextPtr audioContext) 
: videoContext(videoContext)
, audioContext(audioContext) {

}

Decoder::~Decoder() {

}

bool Decoder::decode(Demuxer& demuxer, std::function<void(MEDIA_TYPE, Packet&, Frame&)> func, AVResult* result) {
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

bool Decoder::decodePacket(CodecContextPtr codecContext, Packet& packet, Frame* frame, AVResult* result) {
    int ret = 0;
    if (codecContext == nullptr) {
        return true;
    }
    
    ret = avcodec_send_packet(codecContext->getRawCodecContext(), packet.getRawPacket());
    if (ret < 0) {
        char avErrorMessage[AV_ERROR_MAX_STRING_SIZE] = {0, };
        av_strerror(ret, avErrorMessage, sizeof(avErrorMessage));
        std::cout << "avcodec_send_packet() failed: " << avErrorMessage << std::endl;
        return result->avFailed(ret);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(codecContext->getRawCodecContext(), frame->getRawFrame());
        if (ret < 0) {
            if (ret == AVERROR_EOF || 
                ret == AVERROR(EAGAIN)) {
                return result->success();
            }
            return result->avFailed(ret);
        }

        MEDIA_TYPE mediaType = av::AVMediaTypeToMediaType((int)codecContext->getRawCodecContext()->codec->type);
        this->func(mediaType, packet, *frame);
        
        frame->unref();
    }

    return result->success();
}

};