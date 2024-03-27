#pragma once 

#include "type/FFAVStream.hpp"
#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    enum class DATA_TYPE {
        VIDEO,
        AUDIO,
        SUBTITLE,
        DATA,
        UNKNOWN
    };
    DATA_TYPE DATA_TYPE_FROM_AV_CODEC_TYPE(int codecType);

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

        DATA_TYPE  getType();
        int          getFrameNumber();

        FFAVPacketImplPtr getImpl();

	private:
        int frameNumber;

        DATA_TYPE type;

		FFAVPacketImplPtr packetImpl;
	};
};