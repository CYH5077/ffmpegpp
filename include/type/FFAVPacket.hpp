#pragma once 

#include "type/ffav.hpp"

#include <memory>

namespace ff {
	class FFAVPacket {
	public:
		explicit FFAVPacket();
		explicit FFAVPacket(FFAVPacket& packet);
		explicit FFAVPacket(FFAVPacketImplPtr packetImpl);

		virtual ~FFAVPacket();

	public:
		 FFAVPacketImplPtr getImpl();

	private:
		FFAVPacketImplPtr packetImpl;
	};
};