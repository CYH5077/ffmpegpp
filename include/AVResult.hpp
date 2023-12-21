#pragma once

#include <string>

namespace av {
    class AVResult {
    public:
        explicit AVResult();
        virtual ~AVResult();

    public:
        bool        isSuccess();
        bool        isFileEOF();
        int         getErrorCode();
        std::string getErrorMessage();

        bool success();
        bool success(int avErrorCode);
        bool avFailedFileEOF();
        bool avFailed(int avErrorCode);
        bool failed(int errorCode, std::string&& errorMessage);

    private:
        bool result;

        int errorCode;
        std::string errorMessage;
    };
};