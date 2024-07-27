#include "type/FFAVInputContext.hpp"

#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
}

#include <memory>

namespace ff {
    FFAVInputContext::FFAVInputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
        this->isOpenedFlag = false;

        this->videoStreams = std::make_shared<FFAVStreamList>();
        this->audioStreams = std::make_shared<FFAVStreamList>();
        this->streams = std::make_shared<FFAVStreamList>();
    }

    FFAVInputContext::~FFAVInputContext() {
        this->close();
    }

    AVError FFAVInputContext::open(const std::string& url) {
        this->close();

        // AVFormatContext 메모리 할당.
        AVFormatContext* formatContext = avformat_alloc_context();
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

    AVError FFAVInputContext::open(const std::string&& url) {
        return this->open(url);
    }

    void FFAVInputContext::close() {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();

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
            return AVError(
                AV_ERROR_TYPE::AV_ERROR, "FFAVInputContext is not opened", -1, "FFAVInputContext::readFrame");
        }

        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        AVPacket* packet = ffpacket->getImpl()->getRaw().get();

        // AVFormatContext 에서 AVPacket을 읽어옴.
        int ret = av_read_frame(formatContext, packet);
        // ret이 eof 일경우
        if (ret == AVERROR_EOF) {
            return AVError(AV_ERROR_TYPE::AV_EOF);
        } else if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_read_frame failed", ret, "av_read_frame");
        }

        int codecType = formatContext->streams[packet->stream_index]->codecpar->codec_type;
        ffpacket->setType(DATA_TYPE_FROM_AV_CODEC_TYPE(codecType));
        ffpacket->setDecodeStream(this->streams->at(packet->stream_index));
        
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    int FFAVInputContext::getFrameCount() {
        int totalCount = 0;

        FFAVStreamPtr videoStream = this->videoStreams->size() > 0 ? this->videoStreams->at(0) : nullptr;
        FFAVStreamPtr audioStream = this->audioStreams->size() > 0 ? this->audioStreams->at(0) : nullptr;

        totalCount += videoStream ? videoStream->getImpl()->getRaw()->nb_frames : 0;
        totalCount += audioStream ? audioStream->getImpl()->getRaw()->nb_frames : 0;

        return totalCount;
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

    FFAVCodecParametersPtr FFAVInputContext::getCodecParameters(int index) {
        FFAVCodecParametersPtr codecParameters = FFAVCodecParameters::create();
        codecParameters->getImpl()->setRaw(this->formatContextImpl->getRaw()->streams[index]->codecpar);
        return codecParameters;
    }

    FFAVStreamListPtr FFAVInputContext::getStreams() {
		return this->streams;
	}

    FFAVStreamListPtr FFAVInputContext::getVideoStreams() {
        return this->videoStreams;
    }

    FFAVStreamListPtr FFAVInputContext::getAudioStreams() {
        return this->audioStreams;
    }

    AVError FFAVInputContext::setOpt(const std::string& key, const std::string& value) {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        int ret = av_opt_set(formatContext->priv_data, key.c_str(), value.c_str(), 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_opt_set failed", ret, "av_opt_set");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVInputContextIterator FFAVInputContext::begin() {
        return FFAVInputContextIterator(this);
    }

    FFAVInputContextIterator FFAVInputContext::end() {
        return FFAVInputContextIterator();
    }

    void FFAVInputContext::findMetaData() {
        // nb streams 만큼 순회하면서 vector에 데이터를 넣는다
        int streamIndex = 0;
        for (int i = 0; i < this->getStreamsCount(); i++) {
            AVStream* stream = this->formatContextImpl->getRaw()->streams[i];

            FFAVStreamPtr ffavStream = FFAVStream::create(DATA_TYPE::UNKNOWN);
            ffavStream->getImpl()->setRaw(stream);
            ffavStream->setStreamIndex(streamIndex);

            if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                ffavStream->setDataType(DATA_TYPE::VIDEO);
                this->videoStreams->emplace_back(ffavStream);
            } else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                ffavStream->setDataType(DATA_TYPE::AUDIO);
                this->audioStreams->emplace_back(ffavStream);
            }
            this->streams->emplace_back(ffavStream);

            streamIndex++;
        }
    }

    //////////////////////// Iterator
    FFAVInputContextIterator::FFAVInputContextIterator(FFAVInputContext* context) : context(context) {
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
    FFAVPacket& FFAVInputContextIterator::operator->() {
        return currentPacket;
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
