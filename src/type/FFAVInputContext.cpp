#include "type/FFAVInputContext.hpp"

#include "type/FFAVDecodeStream.hpp"
#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "utils/createDecodeContext.hpp"
#include "error/AVDemuxException.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
}

#include <memory>

namespace ff {
    FFAVInputContext::FFAVInputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVInputContext::~FFAVInputContext() {
        this->close();
    }

    FFAVFormatContextImplPtr FFAVInputContext::getImpl() {
        return this->formatContextImpl;
    }

    AVError FFAVInputContext::open(const std::string& url, bool cudaDecode) {
        this->close();

        // AVFormatContext alloc
        AVFormatContext* avFormatContext = avformat_alloc_context();
        if (avFormatContext == nullptr) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_alloc_context failed", -1, "avformat_alloc_context");
        }

        // File open
        int ret = avformat_open_input(&avFormatContext, url.c_str(), nullptr, nullptr);
        if (ret < 0) {
            avformat_free_context(avFormatContext);
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " open failed", ret, "avformat_open_input");
        }

        // Stream find
        ret = avformat_find_stream_info(avFormatContext, nullptr);
        if (ret < 0) {
            avformat_close_input(&avFormatContext);
            avformat_free_context(avFormatContext);
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " find stream info failed", ret, "avformat_find_stream_info");
        }

        this->formatContextImpl->setRaw(avFormatContext);
        this->parseStreamInfo(cudaDecode);

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    bool FFAVInputContext::isOpened() {
        return this->formatContextImpl->getRaw() != nullptr;
    }

    void FFAVInputContext::close() {
        this->decodeStreamList = nullptr;

        AVFormatContext* formatContext = this->formatContextImpl->getRaw();

        if (formatContext != nullptr) {
            avformat_close_input(&formatContext);
            avformat_free_context(formatContext);
        }

        this->formatContextImpl->setRaw(nullptr);
    }

    AVError FFAVInputContext::demux(FFAVPacket* packet) {
        if (!this->isOpened()) {
            return AVError(
                AV_ERROR_TYPE::AV_ERROR, "FFAVInputContext is not opened", -1, "FFAVInputContext::readFrame");
        }

        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        AVPacket* avPacket = packet->getImpl()->getRaw().get();

        // AVFormatContext 에서 AVPacket을 읽어옴.
        int ret = av_read_frame(formatContext, avPacket);
        // ret이 eof 일경우 null packet을 반환
        if (ret == AVERROR_EOF) {
            packet->getImpl()->getRaw() = nullptr;
            return AVError(AV_ERROR_TYPE::AV_EOF);
        } else if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_read_frame failed", ret, "av_read_frame");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVDecodeStreamListPtr FFAVInputContext::getDecodeStreamList() {
        return this->decodeStreamList;
    }

    FFAVDecodeStreamListPtr FFAVInputContext::getVideoDecodeStreamList() {
        FFAVDecodeStreamListPtr videoDecodeStreamList = std::make_shared<FFAVDecodeStreamList>();

        for (auto iter : *this->decodeStreamList) {
            if (iter->getType() == DATA_TYPE::VIDEO) {
                videoDecodeStreamList->emplace_back(iter);
            }
        }

        return videoDecodeStreamList;
    }

    FFAVDecodeStreamListPtr FFAVInputContext::getAudioDecodeStreamList() {
        FFAVDecodeStreamListPtr audioDecodeStreamList = std::make_shared<FFAVDecodeStreamList>();

        for (auto iter : *this->decodeStreamList) {
            if (iter->getType() == DATA_TYPE::AUDIO) {
                audioDecodeStreamList->emplace_back(iter);
            }
        }

        return audioDecodeStreamList;
    }

    FFAVInputContextIterator FFAVInputContext::begin() {
        return FFAVInputContextIterator(this);
    }

    FFAVInputContextIterator FFAVInputContext::end() {
        return FFAVInputContextIterator();
    }

    AVError FFAVInputContext::parseStreamInfo(bool cudaDecode) {
        this->decodeStreamList = std::make_shared<FFAVDecodeStreamList>();

        AVFormatContext* avFormatContext = this->formatContextImpl->getRaw();

        AVError error;
        int streamIndex = 0;
        for (int i = 0; i < avFormatContext->nb_streams; i++) {
            AVStream* avStream = avFormatContext->streams[i];

            FFAVDecodeStreamPtr decodeStream = FFAVDecodeStream::create(DATA_TYPE::UNKNOWN);
            decodeStream->getImpl()->setRaw(avStream);
            decodeStream->setStreamIndex(streamIndex++);

            auto codecType = avStream->codecpar->codec_type;
            if (codecType == AVMEDIA_TYPE_VIDEO) {  // Video
                decodeStream->setType(DATA_TYPE::VIDEO);
            } else if (codecType == AVMEDIA_TYPE_AUDIO) {  // Audio
                decodeStream->setType(DATA_TYPE::AUDIO);
            } else {  // Other
                decodeStream->setType(DATA_TYPE::UNKNOWN);
            }

            // Decode context create
            if (codecType == AVMEDIA_TYPE_VIDEO) {  // Video
                FFAVCodecContextPtr decodeContext;
                if (cudaDecode) {
                    decodeContext = video::decode::createCUDACodecContext(decodeStream, &error);
                } else {
                    decodeContext = video::decode::createCodecContext(decodeStream, &error);
                }

                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
                }
                decodeStream->setCodecContext(decodeContext);
            } else if (codecType == AVMEDIA_TYPE_AUDIO) {  // Audio
                FFAVCodecContextPtr decodeContext = audio::decode::createCodecContext(decodeStream, &error);
                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
                }
                decodeStream->setCodecContext(decodeContext);
            }

            this->decodeStreamList->emplace_back(decodeStream);
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    //////////////////////// Iterator
    FFAVInputContextIterator::FFAVInputContextIterator(FFAVInputContext* context) : context(context) {
        this->eofFlag = false;
        if (this->context && this->context->isOpened()) {
            // 초기 패킷 읽기 시도
            if (this->context->demux(&this->currentPacket).getType() != AV_ERROR_TYPE::SUCCESS) {
                // 실패한 경우, context를 nullptr로 설정하여 반복자의 끝을 나타냄
                this->context = nullptr;
            }
        } else {
            this->context = nullptr;
        }
    }

    FFAVPacket& FFAVInputContextIterator::operator*() {
        return this->currentPacket;
    }

    FFAVPacket& FFAVInputContextIterator::operator->() {
        return this->currentPacket;
    }

    FFAVInputContextIterator& FFAVInputContextIterator::operator++() {
        if (this->context != nullptr) {
            AVError error = this->context->demux(&this->currentPacket);
            if (error.getType() == AV_ERROR_TYPE::AV_EOF) {
                if (eofFlag == false) {
                    eofFlag = true;
                } else {
                    // 더 이상 패킷을 읽을 수 없으면 반복자를 끝으로 설정
                    this->context = nullptr;
                }
            } else if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                // 다음 패킷을 읽는 데 실패하면 반복자를 끝으로 설정
                throw AVDemuxException(error);
            }
        }
        return *this;
    }

    bool operator==(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b) {
        return a.context == b.context;
    }

    bool operator!=(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b) {
        return a.context != b.context;
    }
};
