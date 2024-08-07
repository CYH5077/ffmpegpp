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
        this->errorAVMessage = this->getAVErrorMessage(errorAVCode);
    }

    AV_ERROR_TYPE AVError::getType() const {
        return this->errorType;
    }

    std::string AVError::getMessage() const {
        return this->errorMessage;
    }

    int AVError::getAVCode() const {
        return this->errorAVCode;
    }

    std::string AVError::getAVFunction() const {
        return this->errorAVFunction;
    }

    std::string AVError::getAVErrorMessage() const {
        return this->errorAVMessage;
    }

    std::string AVError::getAVErrorMessage(int errorAVCode) {
        char avErrorMessage[AV_ERROR_MAX_STRING_SIZE] = {
            0,
        };
        av_strerror(errorAVCode, avErrorMessage, sizeof(avErrorMessage));
        return avErrorMessage;
    }
};
