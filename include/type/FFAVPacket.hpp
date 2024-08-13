#pragma once

#include <memory>
#include <vector>

#include "common/FFAVType.hpp"
#include "type/FFAVStream.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVDecodeStream;
    class FFAVEncodeStream;
    using FFAVDecodeStreamPtr = std::shared_ptr<FFAVDecodeStream>;
    using FFAVEncodeStreamPtr = std::shared_ptr<FFAVEncodeStream>;

    class FFAVPacket {
    public:
        explicit FFAVPacket();
        virtual ~FFAVPacket();

    public:
        FFAVPacketImplPtr getImpl();

        int getStreamIndex();
        int getFrameNumber();

        void setFrameNumber(int frameNumber);
        void setStreamIndex(int streamIndex);

    public:
        void rescaleTS(FFAVDecodeStreamPtr srcStream, FFAVEncodeStreamPtr dstStream);

    private:
        int frameNumber;

        FFAVPacketImplPtr packetImpl;
    };

    // FFAVPacketList
    class FFAVPacketList : public std::vector<FFAVPacket> {
    public:
        virtual ~FFAVPacketList();
    };

    using FFAVPacketListPtr = std::shared_ptr<FFAVPacketList>;
};
