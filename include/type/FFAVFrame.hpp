#pragma once

#include "type/ffav.hpp"

#include <memory>

namespace ff {
    class FFAVFrame {
    public:
        explicit FFAVFrame();
        explicit FFAVFrame(FFAVFrame& frame);
        explicit FFAVFrame(FFAVFrameImplPtr frameImpl);

        virtual ~FFAVFrame();

    public:
        FFAVFrameImplPtr getImpl();

    private:
        FFAVFrameImplPtr frameImpl;
    };
};