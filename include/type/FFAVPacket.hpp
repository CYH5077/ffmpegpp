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

    // FFAVPacketList
    class FFAVPacketList : public std::vector<FFAVPacket> {
    public:
        virtual ~FFAVPacketList();
    };
    using FFAVPacketListPtr = std::shared_ptr<FFAVPacketList>;
};
