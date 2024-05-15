#include "superess/EDSRSuperess.hpp"

namespace ff::dnn {
    EDSRSuperess::EDSRSuperess() {

    }

    EDSRSuperess::~EDSRSuperess() {

    }

    SUPERESS_MODEL EDSRSuperess::getModelType() {
		return SUPERESS_MODEL::EDSR;
	}
};