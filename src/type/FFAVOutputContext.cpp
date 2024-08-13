#include "type/FFAVOutputContext.hpp"

#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
}

#include <memory>
#include <thread>

namespace ff {
    FFAVOutputContext::FFAVOutputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVOutputContext::~FFAVOutputContext() {
        this->close();
    }

    AVError FFAVOutputContext::setOpt(const std::string& key, const std::string& value) {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        int ret = av_opt_set(formatContext->priv_data, key.c_str(), value.c_str(), 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_opt_set failed", ret, "av_opt_set");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVFormatContextImplPtr FFAVOutputContext::getImpl() {
        return this->formatContextImpl;
    }

    AVError FFAVOutputContext::open(const std::string& filename) {
        AVFormatContext* formatContext = nullptr;

        int ret = avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filename.c_str());
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR,
                           "avformat_alloc_output_context2 failed",
                           ret,
                           "avformat_alloc_output_context2");
        }
        this->formatContextImpl->setRaw(formatContext);

        if (!(formatContext->flags & AVFMT_NOFILE)) {
            int ret = avio_open(&formatContext->pb, filename.c_str(), AVIO_FLAG_WRITE);
            if (ret < 0) {
                return AVError(AV_ERROR_TYPE::AV_ERROR, "avio_open failed", ret, "avio_open");
            }
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    void FFAVOutputContext::close() {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();

        if (formatContext != nullptr) {
            // av_write_trailer 호출 전에 formatContext가 nullptr인지 체크
            if (formatContext->pb != nullptr) {
                av_write_trailer(formatContext);
            }

            if (!(formatContext->flags & AVFMT_NOFILE)) {
                avio_closep(&formatContext->pb);
            }
            avformat_free_context(formatContext);
        }

        this->formatContextImpl->setRaw(nullptr);
    }

    AVError FFAVOutputContext::writeHeader() {
        int ret = avformat_write_header(this->formatContextImpl->getRaw(), nullptr);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_write_header failed", ret, "avformat_write_header");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVOutputContext::writePacket(FFAVPacketListPtr packetList) {
        for (auto& packet : *packetList) {
            FFAVDecodeStreamPtr decodeStream = this->streamList[packet.getStreamIndex()].first;
            FFAVEncodeStreamPtr encodeStream = this->streamList[packet.getStreamIndex()].second;
            packet.rescaleTS(decodeStream, encodeStream);

            AVPacket* avPacket = packet.getImpl()->getRaw().get();
            int ret = av_interleaved_write_frame(this->formatContextImpl->getRaw(), avPacket);
            if (ret < 0) {
				return AVError(AV_ERROR_TYPE::AV_ERROR, "av_interleaved_write_frame failed", ret, "av_interleaved_write_frame");
			}
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVEncodeStreamPtr FFAVOutputContext::addStream(HW_VIDEO_CODEC videoCodec, FFAVDecodeStreamPtr stream) {
        FFAVEncodeStreamPtr encodeStream = FFAVEncodeStream::create(DATA_TYPE::VIDEO);
        encodeStream->setCodec(videoCodec);
        return this->createStream(encodeStream, stream, true);
    }

    FFAVEncodeStreamPtr FFAVOutputContext::addStream(VIDEO_CODEC videoCodec, FFAVDecodeStreamPtr stream) {
        FFAVEncodeStreamPtr encodeStream = FFAVEncodeStream::create(DATA_TYPE::VIDEO);
        encodeStream->setCodec(videoCodec);
        return this->createStream(encodeStream, stream, true);
    }

    FFAVEncodeStreamPtr FFAVOutputContext::addStream(AUDIO_CODEC audioCodec, FFAVDecodeStreamPtr stream) {
        FFAVEncodeStreamPtr encodeStream = FFAVEncodeStream::create(DATA_TYPE::AUDIO);
        encodeStream->setCodec(audioCodec);
        return this->createStream(encodeStream, stream, false);
    }

    FFAVEncodeStreamPtr FFAVOutputContext::createStream(FFAVEncodeStreamPtr encodeStream,
                                                        FFAVDecodeStreamPtr decodeStream,
                                                        bool isVideo) {
        /// Create Stream
        AVStream* avEncodeStream = avformat_new_stream(this->formatContextImpl->getRaw(), nullptr);
        if (avEncodeStream == nullptr) {
            return nullptr;
        }
        encodeStream->getImpl()->setRaw(avEncodeStream);

        AVStream* avDecodeStream = decodeStream->getImpl()->getRaw();
        int ret = avcodec_parameters_copy(avEncodeStream->codecpar, avDecodeStream->codecpar);
        if (ret < 0) {
            return nullptr;
        }

        FFAVCodecContextPtr encodeCodecContext = this->createCodecContext(encodeStream, decodeStream, isVideo);
        if (encodeCodecContext == nullptr) {
            return nullptr;
        }

        encodeStream->setCodecContext(encodeCodecContext);
        encodeStream->setStreamIndex(this->streamList.size());

        this->streamList.emplace_back(std::make_pair(decodeStream, encodeStream));
        
        return encodeStream;
    }

    FFAVCodecContextPtr FFAVOutputContext::createCodecContext(FFAVEncodeStreamPtr encodeStream,
                                                              FFAVDecodeStreamPtr decodeStream,
                                                              bool isVideo) {
        FFAVCodecContextPtr encodeCodecContext = FFAVCodecContext::create();

        // Codec name setting
        std::string codecName;
        if (encodeStream->isVideoStream()) {
            if (encodeStream->getHWVideoCodec() != HW_VIDEO_CODEC::NONE) {
				codecName = HW_VIDEO_CODEC_TO_STRING(encodeStream->getHWVideoCodec());
			} else {
				codecName = VIDEO_CODEC_TO_STRING(encodeStream->getVideoCodec());
			}
        } else if (encodeStream->isAudioStream()) {
            codecName = AUDIO_CODEC_TO_STRING(encodeStream->getAudioCodec());
        } else {
            return nullptr;
        }

        // Codec find
        const AVCodec* avCodec = avcodec_find_encoder_by_name(codecName.c_str());
        if (avCodec == nullptr) {
            return nullptr;
        }

        // AVCodecContext alloc
        AVCodecContext* avCodecContext = avcodec_alloc_context3(avCodec);
        if (avCodec == nullptr) {
            return nullptr;
        }
        encodeCodecContext->setCodecName(codecName);
        encodeCodecContext->getImpl()->setRaw(avCodecContext);

        AVCodecContext* avDecodeCodecContext = decodeStream->getCodecContext()->getImpl()->getRaw();
        AVCodecParameters* avDecodeCodecParameters = decodeStream->getImpl()->getRaw()->codecpar;
        AVStream* avDecodeStream = decodeStream->getImpl()->getRaw();
        if (isVideo == true) {
            avCodecContext->width = avDecodeCodecParameters->width;
            avCodecContext->height = avDecodeCodecParameters->height;

            if (avCodec->pix_fmts) {
                avCodecContext->pix_fmt = avCodec->pix_fmts[0];
            } else {
                avCodecContext->pix_fmt = avDecodeCodecContext->pix_fmt;
            }
            avCodecContext->framerate = avDecodeStream->avg_frame_rate;

            avCodecContext->gop_size = avDecodeCodecContext->gop_size;
            avCodecContext->max_b_frames = avDecodeCodecContext->max_b_frames;
            avCodecContext->sample_aspect_ratio = avDecodeCodecContext->sample_aspect_ratio;
        }

        if (isVideo == false) {  // Audio
            avCodecContext->sample_rate = avDecodeCodecParameters->sample_rate;
            av_channel_layout_copy(&avCodecContext->ch_layout, &avDecodeCodecParameters->ch_layout);
            avCodecContext->sample_fmt = (AVSampleFormat)avDecodeCodecParameters->format;
        }

        avCodecContext->bit_rate = avDecodeCodecParameters->bit_rate;
        avCodecContext->time_base = avDecodeStream->time_base;

        avCodecContext->thread_count = std::thread::hardware_concurrency();

        return encodeCodecContext;
    }
};
