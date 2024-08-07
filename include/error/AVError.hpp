#pragma once

#include <string>

#include "error/AVErrorTypes.hpp"

namespace ff {
    class AVError {
    public:
        explicit AVError();
        explicit AVError(AV_ERROR_TYPE errorType);
        explicit AVError(AV_ERROR_TYPE errorType, const std::string& errorMessage);
        explicit AVError(AV_ERROR_TYPE errorType,
                         const std::string& errorMessage,
                         int errorAVCode,
                         const std::string& errorAVFunction);
        virtual ~AVError();

    public:
        void setError(AV_ERROR_TYPE errorType,
                      const std::string& errorMessage,
                      int errorAVCode,
                      const std::string& errorAVFunction);

        AV_ERROR_TYPE getType() const;
        std::string getMessage() const;
        int getAVCode() const;
        std::string getAVFunction() const;
        std::string getAVErrorMessage() const;

    private:
        std::string getAVErrorMessage(int errorAVCode);

    private:
        AV_ERROR_TYPE errorType;
        std::string errorMessage;

        int errorAVCode;
        std::string errorAVFunction;
        std::string errorAVMessage;
    };
};
