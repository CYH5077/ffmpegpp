#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
	class FFAVFormatContextImpl : public std::enable_shared_from_this<FFAVFormatContextImpl> {
	public:
		explicit FFAVFormatContextImpl();
		virtual ~FFAVFormatContextImpl();

	public: // »ý¼º
		static FFAVFormatContextImplPtr create();

	public:
		// Raw AVFormatContext
		AVFormatContext* getRaw();
		void			 setRaw(AVFormatContext* formatContext);

	public:
		// Raw AVFormatContext
		AVFormatContext* formatContext;
	};
};