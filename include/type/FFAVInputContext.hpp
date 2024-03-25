#pragma once

#include "type/ffav.hpp"

#include "error/AVError.hpp"

#include <memory>

namespace ff {
    class FFAVInputContext {
    public:
        explicit FFAVInputContext();

        virtual ~FFAVInputContext();

    public:
        AVError open(const std::string& url);
        AVError open(const std::string&& url);
        void close();

    public:
        FFAVFormatContextImplPtr getImpl();

    private:
        FFAVFormatContextImplPtr formatContextImpl;
    };
};