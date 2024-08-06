#pragma once

#include <memory>
#include <vector>

#include "common/FFAVType.hpp"
#include "type/FFAVStream.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVPacket {
    public:
        explicit FFAVPacket();
        virtual ~FFAVPacket();

    public:
        FFAVPacketImplPtr getImpl();

        int getStreamIndex();
        void setStreamIndex(int streamIndex);

    private:
        FFAVPacketImplPtr packetImpl;
    };

    using FFAVPacketList = std::vector<FFAVPacket>;
    using FFAVPacketListPtr = std::shared_ptr<FFAVPacketList>;
};
