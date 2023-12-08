#include "Muxer.hpp"

#include "CodecContext.hpp"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
}

namespace av {

Muxer::Muxer() {
    this->formatContext = nullptr;
}

Muxer::~Muxer() {
    this->close();
}

bool Muxer::mux(Demuxer& demuxer, const std::string& saveFileName, AVResult* result) {
    if (result == nullptr) {
        return false;
    }

    if (!this->open(saveFileName, result)) {
        return result->isSuccess();
    }

    if (!this->copyStreamsFrom(demuxer, result)) {
        return result->isSuccess();
    }

    if (!this->writeHeader(result)) {
        return result->isSuccess();
    }

    if (!this->copyPacketsFrom(demuxer, result)) {
        return result->isSuccess();
    }

    return result->success();
}

bool Muxer::open(const std::string& fileName, AVResult* result) {
    if (result == nullptr) {
        return false;
    }

    avformat_alloc_output_context2(&this->formatContext, nullptr, nullptr, fileName.c_str());
    if (this->formatContext == nullptr) {
        return result->avFailed(AVERROR(ENOMEM));
    }

    if (!(this->formatContext->flags & AVFMT_NOFILE)) {
        int ret = avio_open(&this->formatContext->pb, fileName.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            return result->avFailed(ret);
        }
    }
    
    return result->success();
}

void Muxer::close() {
    if (this->formatContext != nullptr) {
        av_write_trailer(this->formatContext);
        if (!(this->formatContext->flags & AVFMT_NOFILE)) {
            avio_closep(&this->formatContext->pb);
        }
        avformat_free_context(this->formatContext);
    }
    this->formatContext = nullptr;
}

bool Muxer::copyStreamsFrom(Demuxer& demuxer, AVResult* result) {
    if (result == nullptr) {
        return false;
    }

    int streamIndex = 0;
    this->streamsMapper.resize(demuxer.getStreamCount(), 0);
    for (unsigned int i = 0; i < demuxer.getStreamCount(); i++) {
        AVCodecParameters* demuxerCodecParameter = demuxer.getRawStream(i)->codecpar;
        if (demuxerCodecParameter->codec_type != AVMEDIA_TYPE_VIDEO &&
            demuxerCodecParameter->codec_type != AVMEDIA_TYPE_AUDIO &&
            demuxerCodecParameter->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                this->streamsMapper[i] = -1;
            continue;
        }

        this->streamsMapper[i] = streamIndex++;

        AVStream* outputStream = avformat_new_stream(this->formatContext, nullptr);
        if (outputStream == nullptr) {
            return result->avFailed(AVERROR(ENOMEM));
        }

        int ret = avcodec_parameters_copy(outputStream->codecpar, demuxerCodecParameter);
        if (ret < 0) {
            return result->avFailed(ret);
        }

        outputStream->codecpar->codec_tag = 0;
    }
    return result->success();
}

bool Muxer::createNewStream(CodecContextPtr codecContext, AVResult* result) {
    const AVCodec* codec = codecContext->getRawCodecContext()->codec;
    AVStream* stream = avformat_new_stream(this->formatContext, codec);
    if (stream == nullptr) {
        return result->avFailed(AVERROR(ENOMEM));
    }

    int ret = avcodec_parameters_from_context(stream->codecpar, codecContext->getRawCodecContext());
    if (ret < 0) {
        return result->avFailed(ret);
    }

    return result->success();
}

bool Muxer::writeHeader(AVResult* result) {
    if (result == nullptr) {
        return false;
    }

    int ret = avformat_write_header(this->formatContext, nullptr);
    if (ret < 0) {
        return result->avFailed(ret);
    }

    return result->success();
}

bool Muxer::writePacket(Packet& packet, AVResult* result) {
    int ret = av_interleaved_write_frame(this->formatContext, packet.getRawPacket());
    if (ret != 0) {
        return result->avFailed(ret);
    }
    return result->success();
}

AVFormatContext* Muxer::getRawFormatContext() {
    return this->formatContext;
}

AVStream* Muxer::getRawStream(MEDIA_TYPE type) {
    AVMediaType mediaType = (AVMediaType)av::mediaTypeToAVMediaType(type);
    int streamIndex = av_find_best_stream(this->formatContext, mediaType, -1, -1, nullptr, 0);
    if (streamIndex < 0) {
        return nullptr;
    }

    return this->formatContext->streams[streamIndex];
}

bool Muxer::copyPacketsFrom(Demuxer& demuxer, AVResult* result) {
    Packet packet;
    while (demuxer.read(&packet, result)) {
        AVStream* inputStream = demuxer.getRawStream(packet.getStreamIndex());
        if (packet.getStreamIndex() >= (int)this->streamsMapper.size() ||
            this->streamsMapper[packet.getStreamIndex()] < 0) {
            packet.unref();
            continue;
        }

        packet.setStreamIndex(this->streamsMapper[packet.getStreamIndex()]);
        
        // copy packet
        AVStream* outputStream = this->formatContext->streams[packet.getStreamIndex()];
        av_packet_rescale_ts(packet.getRawPacket(), inputStream->time_base, outputStream->time_base);
        packet.setPos(-1);

        int ret = av_interleaved_write_frame(this->formatContext, packet.getRawPacket());
        if (ret < 0) {
            return result->avFailed(ret);
        }
    }

    return result->success();
}

};