#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libswscale/swscale.h"
}

namespace ff {
	class FFAVSwsContextImpl : public std::enable_shared_from_this<FFAVSwsContextImpl> {
	public:
		explicit FFAVSwsContextImpl();
		virtual ~FFAVSwsContextImpl();

	public:
		static FFAVSwsContextImplPtr create();

	public:
		// Raw AVSwsContext
		SwsContext* getRaw();
		void        setRaw(SwsContext* swsContext);

	public:
		SwsContext* swsContext;
	};
}