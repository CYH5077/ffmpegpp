#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    using AVPacketPtr = std::shared_ptr<AVPacket>;

    class FFAVPacketImpl : public std::enable_shared_from_this<FFAVPacketImpl> {
    public:
        explicit FFAVPacketImpl();
        virtual ~FFAVPacketImpl();

    public:  // create
        static FFAVPacketImplPtr create();

    public:  // get set
        AVPacketPtr getRaw();

    private:
        AVPacketPtr packet;
    };
}
