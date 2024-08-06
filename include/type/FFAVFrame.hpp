#pragma once

#include <memory>
#include <vector>

#include "common/FFAVType.hpp"
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

    using FFAVFrameList = std::vector<FFAVFrame>;
    using FFAVFrameListPtr = std::shared_ptr<FFAVFrameList>;
};
