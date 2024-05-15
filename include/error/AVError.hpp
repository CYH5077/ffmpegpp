#pragma once

#include "error/AVErrorTypes.hpp"

#include <string>

namespace ff {
    class AVError {
    public:
        explicit AVError();
        explicit AVError(AV_ERROR_TYPE errorType);
        explicit AVError(AV_ERROR_TYPE errorType, const std::string& errorMessage);
        explicit AVError(AV_ERROR_TYPE errorType, const std::string& errorMessage,  int errorAVCode, const std::string& errorAVFunction);
        virtual ~AVError();

    public:
        void setError(AV_ERROR_TYPE errorType, const std::string& errorMessage,  int errorAVCode, const std::string& errorAVFunction);\

        AV_ERROR_TYPE getType();
        std::string   getMessage();
        int           getAVCode();
        std::string   getAVFunction();
        std::string   getAVErrorMessage();

    private:
        std::string getAVErrorMessages(int errorAVCode);

    private:
        AV_ERROR_TYPE errorType;
        std::string errorMessage;

        int         errorAVCode;
        std::string errorAVFunction;
        std::string errorAVMessage;
    };
};