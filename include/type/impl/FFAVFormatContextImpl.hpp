#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
	typedef std::shared_ptr<AVFormatContext> AVFormatContextPtr;

	class FFAVFormatContextImpl : public std::enable_shared_from_this<FFAVFormatContextImpl> {
	public:
		explicit FFAVFormatContextImpl();
		explicit FFAVFormatContextImpl(FFAVFormatContextImplPtr formatContextImpl);
		virtual ~FFAVFormatContextImpl();

	public: // »ý¼º
		static FFAVFormatContextImplPtr create();
		static FFAVFormatContextImplPtr create(FFAVFormatContextImplPtr formatContextImpl);

	public:
		// Raw AVFormatContext
		AVFormatContextPtr getRaw();
		void			   setRaw(AVFormatContextPtr formatContext);

	public:
		// Raw AVFormatContext
		AVFormatContextPtr formatContext;
	};
};