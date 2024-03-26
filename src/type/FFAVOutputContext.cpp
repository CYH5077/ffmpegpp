#include "type/FFAVOutputContext.hpp"

#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    FFAVOutputContext::FFAVOutputContext() {
        this->isOpenFlag = false;
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVOutputContext::~FFAVOutputContext() {
        this->close();
    }

    AVError FFAVOutputContext::open(std::string& filename) {
        AVFormatContext* formatContext = nullptr;
        int ret = avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filename.c_str());
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_alloc_output_context2 failed", ret, "avformat_alloc_output_context2");
        }
        this->formatContextImpl->setRaw(formatContext);

        if (!(formatContext->flags & AVFMT_NOFILE)) {
            int ret = avio_open(&formatContext->pb, filename.c_str(), AVIO_FLAG_WRITE);
            if (ret < 0) {
                return AVError(AV_ERROR_TYPE::AV_ERROR, "avio_open failed", ret, "avio_open");
            }
        }

        this->isOpenFlag = true;
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVStreamPtr FFAVOutputContext::addStream(ff::FFAVCodecParametersPtr codecParameters) {
        AVStream* stream = avformat_new_stream(this->formatContextImpl->getRaw(), nullptr);
        if (!stream) {
            return nullptr;
        }

        FFAVStreamPtr ffavStream = FFAVStream::create();
        ffavStream->getImpl()->setRaw(stream);

        AVCodecParameters* codecParametersRaw = codecParameters->getImpl()->getRaw();
        int ret = avcodec_parameters_copy(stream->codecpar, codecParametersRaw);
        if (ret < 0) {
            return nullptr;
        }

        return ffavStream;
    }

    AVError FFAVOutputContext::writeHeader() {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        int ret = avformat_write_header(formatContext, nullptr);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_write_header failed", ret, "avformat_write_header");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVOutputContext::writePacket(ff::FFAVPacket& ffavPacket) {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        AVPacket* packet = ffavPacket.getImpl()->getRaw().get();
        int ret = av_interleaved_write_frame(formatContext, packet);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_interleaved_write_frame failed", ret, "av_interleaved_write_frame");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVOutputContext::open(std::string &&filename) {
        return this->open(filename);
    }

    void FFAVOutputContext::close() {
        AVFormatContext* formatContext = this->formatContextImpl->getRaw();

        if (formatContext && this->isOpenFlag) {\
            avformat_close_input(&formatContext);
        } else if (formatContext && !this->isOpenFlag) {
            avformat_free_context(formatContext);
        }

        this->isOpenFlag = false;
        this->formatContextImpl->setRaw(nullptr);
    }

    FFAVFormatContextImplPtr FFAVOutputContext::getImpl() {
        return this->formatContextImpl;
    }

    FFAVStreamPtr FFAVOutputContext::getStream(int index) {
        FFAVStreamPtr stream = FFAVStream::create();
        stream->getImpl()->setRaw(this->formatContextImpl->getRaw()->streams[index]);
        return stream;
    }
};