#pragma once

#include "error/AVError.hpp"

namespace ff {
    class AVDemuxException : public std::exception {
    public:
        AVDemuxException(const AVError& error) : error(error) {}

        AVError getAVError() {
            return error;
        }

        const char* what() const noexcept override {
            return this->error.getAVErrorMessage().c_str();
        }

    private:
        AVError error;
    };
};
