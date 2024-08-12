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

    // FFAVFrameList
    class FFAVFrameList : public std::vector<FFAVFrame> {
    public:
        virtual ~FFAVFrameList();
    };

    using FFAVFrameListPtr = std::shared_ptr<FFAVFrameList>;
};
