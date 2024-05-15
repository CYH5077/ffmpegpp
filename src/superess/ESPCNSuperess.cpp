#include "superess/ESPCNSuperess.hpp"

namespace ff::dnn {
	ESPCNSuperess::ESPCNSuperess() {

	}

	ESPCNSuperess::~ESPCNSuperess() {

	}

	SUPERESS_MODEL ESPCNSuperess::getModelType() {
		return SUPERESS_MODEL::ESPCN;
	}
};