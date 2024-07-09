#include "type/FFAVInputContext.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"

#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVChannelLayoutImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    FFAVInputContext::FFAVInputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
        this->isOpenedFlag = false;

        this->videoStreamIndex = -1;
        this->audioStreamIndex = -1;
    }

    FFAVInputContext::~FFAVInputContext() {
        this->close();
    }

    AVError FFAVInputContext::open(const std::string &url) {
        this->close();

        // AVFormatContext 메모리 할당.
        AVFormatContext* formatContext  = avformat_alloc_context();
        if (!formatContext) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_alloc_context failed", -1, "avformat_alloc_context");
        }

        // File open
        int ret = avformat_open_input(&formatContext, url.c_str(), nullptr, nullptr);
        if (ret < 0) {
            avformat_free_context(formatContext);
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " open failed", ret, "avformat_open_input");
        }

        // Stream find
        ret = avformat_find_stream_info(formatContext, nullptr);
        if (ret < 0) {
            avformat_close_input(&formatContext);
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " find stream info failed", ret, "avformat_find_stream_info");
        }


        this->isOpenedFlag = true;
        this->formatContextImpl->setRaw(formatContext);
        this->findMetaData();
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVInputContext::open(const std::string &&url) {
        return this->open(url);
    }

    void FFAVInputContext::close() {
        AVFormatContext* formatContext  = this->formatContextImpl->getRaw();

        // formatContext가 nullptr이 아닐경우
        if (formatContext && isOpenedFlag) {
            avformat_close_input(&formatContext);
        } else if (formatContext && !isOpenedFlag) {
            avformat_free_context(formatContext);
        }

        this->isOpenedFlag = false;
        this->formatContextImpl->setRaw(nullptr);
    }

    AVError FFAVInputContext::readFrame(ff::FFAVPacket* ffpacket) {
        if (!this->isOpened()) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "FFAVInputContext is not opened", -1, "FFAVInputContext::readFrame");
        }

        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        AVPacket* packet                = ffpacket->getImpl()->getRaw().get();

        // AVFormatContext 에서 AVPacket을 읽어옴.
        int ret = av_read_frame(formatContext, packet);
        // ret이 eof 일경우
        if (ret == AVERROR_EOF || avio_feof(formatContext->pb)) {
            return AVError(AV_ERROR_TYPE::AV_EOF);
        } else if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_read_frame failed", ret, "av_read_frame");
        }

        int codecType = formatContext->streams[packet->stream_index]->codecpar->codec_type;
        ffpacket->setType(DATA_TYPE_FROM_AV_CODEC_TYPE(codecType));

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    bool FFAVInputContext::isOpened() const {
        return this->isOpenedFlag;
    }

    int FFAVInputContext::getStreamsCount() {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        if (formatContext == nullptr) {
            return -1;
        }
        return formatContext->nb_streams;
    }

    FFAVFormatContextImplPtr FFAVInputContext::getImpl() {
        return this->formatContextImpl;
    }

    FFAVCodecParametersPtr FFAVInputContext::getVideoCodecParameters() {
        int streamIndex = av_find_best_stream(this->formatContextImpl->getRaw(), AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (streamIndex < 0) {
            return nullptr;
        }
        return this->codecParameters[streamIndex];
    }

    FFAVCodecParametersPtr FFAVInputContext::getAudioCodecParameters() {
        int streamIndex = av_find_best_stream(this->formatContextImpl->getRaw(), AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
        if (streamIndex < 0) {
            return nullptr;
        }
        return this->codecParameters[streamIndex];
    }

    FFAVCodecParametersPtr FFAVInputContext::getCodecParameters(int index) {
        return this->codecParameters[index];
    }

    FFAVStreamPtr FFAVInputContext::getVideoStream() {
        int streamIndex = av_find_best_stream(this->formatContextImpl->getRaw(), AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (streamIndex < 0) {
            return nullptr;
        }
        return this->streams[streamIndex];
    }

    FFAVStreamPtr FFAVInputContext::getAudioStream() {
        int streamIndex = av_find_best_stream(this->formatContextImpl->getRaw(), AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
        if (streamIndex < 0) {
            return nullptr;
        }
        return this->streams[streamIndex];
    }

    FFAVStreamPtr FFAVInputContext::getStream(int index) {
        return this->streams[index];
    }

    FFAVChannelLayoutPtr FFAVInputContext::getAudioChannelLayout() {
        int streamIndex = this->getAudioStreamIndex();

        FFAVChannelLayoutPtr ffavChannelLayoutPtr = FFAVChannelLayout::create();
        ffavChannelLayoutPtr->getImpl()->setRaw(this->formatContextImpl->getRaw()->streams[streamIndex]->codecpar->ch_layout);
        return ffavChannelLayoutPtr;
    }

    int FFAVInputContext::getVideoStreamIndex() {
        return av_find_best_stream(this->formatContextImpl->getRaw(), AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    }

    int FFAVInputContext::getAudioStreamIndex() {
        return av_find_best_stream(this->formatContextImpl->getRaw(), AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    }

    FFAVInputContextIterator FFAVInputContext::begin() {
        return FFAVInputContextIterator(this);
    }

    FFAVInputContextIterator FFAVInputContext::end() {
        return FFAVInputContextIterator();
    }

    void FFAVInputContext::findMetaData() {
        // nb streams 만큼 순회하면서 vector에 데이터를 넣는다
        for (int i = 0; i < this->getStreamsCount(); i++) {
            AVStream* stream = this->formatContextImpl->getRaw()->streams[i];

            FFAVStreamPtr ffavStream = FFAVStream::create();
            ffavStream->getImpl()->setRaw(stream);
            this->streams.push_back(ffavStream);

            FFAVCodecParametersPtr codecParameters = FFAVCodecParameters::create();
            codecParameters->getImpl()->setRaw(stream->codecpar);
            this->codecParameters.push_back(codecParameters);
        }
    }


    //////////////////////// Iterator
    FFAVInputContextIterator::FFAVInputContextIterator(FFAVInputContext* context)
    : context(context) {
        if (context && context->isOpened()) {
            // 초기 패킷 읽기 시도
            if (context->readFrame(&this->currentPacket).getType() != AV_ERROR_TYPE::SUCCESS) {
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
    FFAVPacket* FFAVInputContextIterator::operator->() {
        return &currentPacket;
    }

    FFAVInputContextIterator& FFAVInputContextIterator::operator++() {
        if (context && context->readFrame(&currentPacket).getType() != AV_ERROR_TYPE::SUCCESS) {
            // 다음 패킷을 읽는 데 실패하면 반복자를 끝으로 설정
            context = nullptr;
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