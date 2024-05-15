#include "type/impl/FFAVSwsContextImpl.hpp"

namespace ff {
	FFAVSwsContextImplPtr FFAVSwsContextImpl::create() {
		return std::make_shared<FFAVSwsContextImpl>();
	}


	FFAVSwsContextImpl::FFAVSwsContextImpl() {
		this->swsContext = nullptr;
	}

	FFAVSwsContextImpl::~FFAVSwsContextImpl() {
		if (this->swsContext != nullptr) {
			sws_freeContext(this->swsContext);
		}
	}


	SwsContext* FFAVSwsContextImpl::getRaw() {
		return this->swsContext;
	}

	void FFAVSwsContextImpl::setRaw(SwsContext* swsContext) {
		this->swsContext = swsContext;
	}
}