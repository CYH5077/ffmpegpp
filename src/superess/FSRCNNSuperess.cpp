#include "superess/FSRCNNSuperess.hpp"

namespace ff::dnn {
	FSRCNNSuperess::FSRCNNSuperess() {

	}

	FSRCNNSuperess::~FSRCNNSuperess() {

	}

	SUPERESS_MODEL FSRCNNSuperess::getModelType() {
		return SUPERESS_MODEL::FSRCNN;
	}
}