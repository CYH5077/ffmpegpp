#pragma once

#include "type/ffav.hpp"

#include <memory>

namespace ff {
    class FFAVFormatContext {
    public:
        explicit FFAVFormatContext();
        explicit FFAVFormatContext(FFAVFormatContext& formatContext);
        explicit FFAVFormatContext(FFAVFormatContextImplPtr formatContextImpl);

        virtual ~FFAVFormatContext();

    public:
        FFAVFormatContextImplPtr getImpl();

    private:
        FFAVFormatContextImplPtr formatContextImpl;
    };
};