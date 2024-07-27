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
        FFAVPacketImplPtr getImpl();
        DATA_TYPE getType();
        int getFrameNumber();
        FFAVStreamPtr getDecodeStream();
        FFAVStreamPtr getEncodeStream();
        int getStreamIndex();

        void setType(DATA_TYPE type);
        void setFrameNumber(int frameNumber);
        void setStreamIndex(int streamIndex);
        void setDecodeStream(FFAVStreamPtr decodeStream);
        void setEncodeStream(FFAVStreamPtr encodeStream);

    private:
        void init();

    private:
        int frameNumber;

        DATA_TYPE type;

        FFAVStreamPtr decodeStream;
        FFAVStreamPtr encodeStream;

        FFAVPacketImplPtr packetImpl;
    };
};
