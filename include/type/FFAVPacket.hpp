#pragma once

#include <memory>

#include "common/FFAVDataType.hpp"
#include "type/FFAVStream.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVPacket {
    public:
        explicit FFAVPacket();
        virtual ~FFAVPacket();

    public:
        FFAVPacketImplPtr getImpl();

    private:
        FFAVPacketImplPtr packetImpl;
    };
};
