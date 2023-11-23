#include "InputFormat.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

InputFormat::InputFormat() {
    this->formatContext = nullptr;
}

InputFormat::~InputFormat() {
    this->close();
}

bool InputFormat::open(const std::string& fileName, AVResult& result) {
    if (!this->createFormatContext(result)) {
        return result.isSuccess();
    }

    if (!this->openFormatContext(fileName, result)) {
        return result.isSuccess();
    }

    return result.success();
}

void InputFormat::close() {
    if (this->formatContext != nullptr) {
        avformat_close_input(&this->formatContext);
    }
    this->formatContext = nullptr;
}

void InputFormat::printDump() {
    if (this->formatContext == nullptr) {
        return;
    }

    av_dump_format(this->formatContext, 0, nullptr, 0);
}
AVFormatContext* InputFormat::getRaw() {
    return this->formatContext;
}

bool InputFormat::createFormatContext(AVResult& result) {
    if (this->formatContext != nullptr) {
        this->close();
    }

    this->formatContext = avformat_alloc_context();
    if (this->formatContext == nullptr) {
        return result.failed(-1, "AVFormatContext alloc failed");
    }

    return result.success();
}

bool InputFormat::openFormatContext(const std::string& fileName, AVResult& result) {
    int ret = avformat_open_input(&this->formatContext, fileName.c_str(), nullptr, nullptr);
    if (ret != 0) {
        return result.avFailed(ret);
    }

    ret = avformat_find_stream_info(this->formatContext, nullptr);
    if (ret < 0) {
        return result.avFailed(ret);
    }

    return result.success();
}

};