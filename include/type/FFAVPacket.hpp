#pragma once

#include <memory>

#include "type/FFAVDefine.hpp"
#include "type/FFAVStream.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVPacket {
    public:
        explicit FFAVPacket();
        explicit FFAVPacket(FFAVPacket& packet);
        explicit FFAVPacket(FFAVPacketImplPtr packetImpl);

        virtual ~FFAVPacket();

    public:
        void rescaleTS(FFAVStreamPtr srcStream, FFAVStreamPtr dstStream);

    public:
        void setType(DATA_TYPE type);
        void setFrameNumber(int frameNumber);
        void setStreamIndex(int streamIndex);

        DATA_TYPE getType();
        int getFrameNumber();

        FFAVPacketImplPtr getImpl();

    private:
        int frameNumber;

        DATA_TYPE type;

        FFAVPacketImplPtr packetImpl;
    };
};
