#pragma once

#include "type/ffav.hpp"

#include <memory>

namespace ff {
    class FFAVFrame {
    public:
        explicit FFAVFrame();
        explicit FFAVFrame(const FFAVFrame& frame);
        explicit FFAVFrame(FFAVFrameImplPtr frameImpl);

        virtual ~FFAVFrame();

    public:
        FFAVFrameImplPtr getImpl();

    public:
        void copyTs(FFAVFrame& frame);
        void ref(FFAVFrame& frame);

    private:
        FFAVFrameImplPtr frameImpl;
    };
};