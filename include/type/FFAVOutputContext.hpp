#pragma once

#include "type/ffav.hpp"

#include "error/AVError.hpp"

#include <memory>

namespace ff {
    class FFAVOutputContext {
    public:
        explicit FFAVOutputContext();

        virtual ~FFAVOutputContext();

    public:
        FFAVFormatContextImplPtr getImpl();

    private:
        FFAVFormatContextImplPtr formatContextImpl;
    };
};