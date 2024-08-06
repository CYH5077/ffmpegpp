#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    using AVFramePtr = std::shared_ptr<AVFrame>;

    class FFAVFrameImpl : public std::enable_shared_from_this<FFAVFrameImpl> {
    public:
        explicit FFAVFrameImpl();
        explicit FFAVFrameImpl(FFAVFrameImplPtr frameImpl);

        virtual ~FFAVFrameImpl();

    public: // create
        static FFAVFrameImplPtr create();
        static FFAVFrameImplPtr create(FFAVFrameImplPtr frameImpl);

    public: // get set
        AVFramePtr getRaw();

    private:
        AVFramePtr frame;
    };
};