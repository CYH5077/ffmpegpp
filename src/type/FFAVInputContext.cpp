#include "type/FFAVInputContext.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"

#include "type/impl/FFAVPacketImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    FFAVInputContext::FFAVInputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
        this->isOpenedFlag = false;
    }

    FFAVInputContext::~FFAVInputContext() {
        this->close();
    }

    AVError FFAVInputContext::open(const std::string &url) {
        this->close();

        // AVFormatContext �޸� �Ҵ�.
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
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVInputContext::open(const std::string &&url) {
        return this->open(url);
    }

    void FFAVInputContext::close() {
        AVFormatContext* formatContext  = this->formatContextImpl->getRaw();

        // formatContext�� nullptr�� �ƴҰ��
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

        // AVFormatContext ���� AVPacket�� �о��.
        int ret = av_read_frame(formatContext, packet);
        // ret�� eof �ϰ��
        if (ret == AVERROR_EOF) {
            return AVError(AV_ERROR_TYPE::AV_EOF);
        } else if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_read_frame failed", ret, "av_read_frame");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVInputContextIterator FFAVInputContext::begin() {
        return FFAVInputContextIterator(this);
    }

    FFAVInputContextIterator FFAVInputContext::end() {
        return FFAVInputContextIterator();
    }

    bool FFAVInputContext::isOpened() const {
        return this->formatContextImpl->getRaw() != nullptr;
    }

    FFAVFormatContextImplPtr FFAVInputContext::getImpl() {
        return this->formatContextImpl;
    }



    //////////////////////// Iterator
    FFAVInputContextIterator::FFAVInputContextIterator(FFAVInputContext* context)
    : context(context) {
        if (context && context->isOpened()) {
            // �ʱ� ��Ŷ �б� �õ�
            if (context->readFrame(&this->currentPacket).getType() != AV_ERROR_TYPE::SUCCESS) {
                // ������ ���, context�� nullptr�� �����Ͽ� �ݺ����� ���� ��Ÿ��
                this->context = nullptr;
            }
        } else {
            this->context = nullptr;
        }
    }

    const FFAVPacket& FFAVInputContextIterator::operator*() const {
        return this->currentPacket;

    }
    FFAVPacket* FFAVInputContextIterator::operator->() {
        return &currentPacket;
    }

    FFAVInputContextIterator& FFAVInputContextIterator::operator++() {
        if (context && context->readFrame(&currentPacket).getType() != AV_ERROR_TYPE::SUCCESS) {
            // ���� ��Ŷ�� �д� �� �����ϸ� �ݺ��ڸ� ������ ����
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