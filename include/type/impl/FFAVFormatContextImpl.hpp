#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
	typedef AVFormatContext* AVFormatContextPtr;

	class FFAVFormatContextImpl : public std::enable_shared_from_this<FFAVFormatContextImpl> {
	public:
		explicit FFAVFormatContextImpl();
		virtual ~FFAVFormatContextImpl();

	public: // »ý¼º
		static FFAVFormatContextImplPtr create();

	public:
		// Raw AVFormatContext
		AVFormatContextPtr getRaw();
		void			   setRaw(AVFormatContextPtr formatContext);

	public:
		// Raw AVFormatContext
		AVFormatContextPtr formatContext;
	};
};