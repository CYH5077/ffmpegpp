#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "superess/DnnSuperRes.hpp"

#include "error/ffav.hpp"


namespace ff::dnn {
	class ESPCNSuperess : public DnnSuperRes {
	public:
		explicit ESPCNSuperess();
		virtual ~ESPCNSuperess();

	public:
		virtual SUPERESS_MODEL getModelType() override;
	};
}