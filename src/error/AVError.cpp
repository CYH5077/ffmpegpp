#include "error/AVError.hpp"

extern "C" {
#include "libavutil/error.h"
}

namespace ff {
    AVError::AVError() {
        this->setError(AV_ERROR_TYPE::SUCCESS, "", 0, "");
    }

    AVError::AVError(AV_ERROR_TYPE errorType) {
        this->setError(errorType, "", 0, "");
    }

    AVError::AVError(AV_ERROR_TYPE errorType, const std::string& errorMessage) {
        std::string errorAVFunction = "";
        this->setError(errorType, errorMessage, 0, errorAVFunction);
    }

    AVError::AVError(AV_ERROR_TYPE errorType,
                     const std::string& errorMessage,
                     int errorAVCode,
                     const std::string& errorAVFunction) {
        this->setError(errorType, errorMessage, errorAVCode, errorAVFunction);
    }

    AVError::~AVError() {}

    void AVError::setError(AV_ERROR_TYPE errorType,
                           const std::string& errorMessage,
                           int errorAVCode,
                           const std::string& errorAVFunction) {
        this->errorType = errorType;
        this->errorMessage = errorMessage;
        this->errorAVCode = errorAVCode;
        this->errorAVFunction = errorAVFunction;
        this->errorAVMessage = this->getAVErrorMessages(errorAVCode);
    }

    AV_ERROR_TYPE AVError::getType() {
        return this->errorType;
    }

    std::string AVError::getMessage() {
        return this->errorMessage;
    }

    int AVError::getAVCode() {
        return this->errorAVCode;
    }

    std::string AVError::getAVFunction() {
        return this->errorAVFunction;
    }

    std::string AVError::getAVErrorMessage() {
        return this->errorAVMessage;
    }

    std::string AVError::getAVErrorMessages(int errorAVCode) {
        char avErrorMessage[AV_ERROR_MAX_STRING_SIZE] = {
            0,
        };
        av_strerror(errorAVCode, avErrorMessage, sizeof(avErrorMessage));
        return avErrorMessage;
    }
};
