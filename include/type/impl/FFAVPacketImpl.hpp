#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
	typedef std::shared_ptr<AVPacket> AVPacketPtr;

	class FFAVPacketImpl : public std::enable_shared_from_this<FFAVPacketImpl> {
	public:
		explicit FFAVPacketImpl();
		explicit FFAVPacketImpl(FFAVPacketImplPtr packetImpl);

		virtual ~FFAVPacketImpl();

	public: // 생성
		static FFAVPacketImplPtr create();
		static FFAVPacketImplPtr create(FFAVPacketImplPtr packetImpl);

	public: 
		// Raw AVPacket
		AVPacketPtr getRaw();
		void        setRaw(AVPacketPtr packet);

		// AVPacket 복사
		FFAVPacketImplPtr copy();
		void			  copyFrom(FFAVPacketImplPtr packetImpl);
		void			  copyFrom(AVPacketPtr packet);

	private:
		AVPacketPtr packet;
	};
}