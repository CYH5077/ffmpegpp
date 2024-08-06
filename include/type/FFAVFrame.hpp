#pragma once

#include <memory>

#include "common/FFAVDataType.hpp"
#include "type/impl/ffavimpl.hpp"
#include "type/FFAVStream.hpp"

namespace ff {
    class FFAVFrame {
    public:
        explicit FFAVFrame();
        virtual ~FFAVFrame();

    public:
        FFAVFrameImplPtr getImpl();

    private:
        FFAVFrameImplPtr frameImpl;
    };
};
