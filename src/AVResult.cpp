#include "AVResult.hpp"


extern "C" {
#include "libavutil/error.h"
}

#include <iostream>

namespace av {

    AVResult::AVResult() {
        this->success();
    }

    AVResult::~AVResult() {

    }

    bool AVResult::isSuccess() {
        if (this->result      == false &&
            this->isFileEOF() == true) {
            return true;
        }
        return this->result;
    }

    bool AVResult::isFileEOF() {
        if (this->getErrorCode() == AVERROR_EOF) {
            return true;
        }
        return false;
    }

    int AVResult::getErrorCode() {
        return this->errorCode;
    }

    std::string AVResult::getErrorMessage() {
        return this->errorMessage;
    }

    bool AVResult::success() {
        return this->success(0);
    }

    bool AVResult::success(int avErrorCode) {
        this->result    = true;
        this->errorCode = avErrorCode;
        this->errorMessage = "";

        return this->result;
    }

    bool AVResult::avFailedFileEOF() {
        return this->failed(AVERROR_EOF, "FILE EOF");
    }

    bool AVResult::avFailed(int avErrorCode) {
        char avErrorMessage[AV_ERROR_MAX_STRING_SIZE] = {0, };
        av_strerror(avErrorCode, avErrorMessage, sizeof(avErrorMessage));
        std::cout << "ffmpeg error message: " << avErrorMessage << std::endl;

        return this->failed(avErrorCode, avErrorMessage);
    }

    bool AVResult::failed(int avErrorCode, std::string&& errorMessage) {
        this->result       = false;
        this->errorCode    = avErrorCode;
        this->errorMessage = errorMessage;

        return this->result;
    }

};