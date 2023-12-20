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
        return this->result;
    }

    int AVResult::getErrorCode() {
        return this->errorCode;
    }

    std::string AVResult::getErrorMessage() {
        return this->errorMessage;
    }

    bool AVResult::success() {
        this->result = true;
        this->errorCode = 0;

        return this->result;
    }

    bool AVResult::avFailed(int avErrorCode) {
        char avErrorMessage[AV_ERROR_MAX_STRING_SIZE] = {0, };
        av_strerror(avErrorCode, avErrorMessage, sizeof(avErrorMessage));
        std::cout << avErrorMessage << std::endl;
        return this->failed(avErrorCode, avErrorMessage);
    }

    bool AVResult::failed(int avErrorCode, std::string&& errorMessage) {
        this->result = false;
        this->errorCode = errorCode;
        this->errorMessage = errorMessage;

        return this->result;
    }

};