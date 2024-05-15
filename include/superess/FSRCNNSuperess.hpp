#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "superess/DnnSuperRes.hpp"

#include "error/ffav.hpp"


namespace ff::dnn {
	class FSRCNNSuperess : public DnnSuperRes {
	public:
		explicit FSRCNNSuperess();
		virtual ~FSRCNNSuperess();

	public:
		virtual SUPERESS_MODEL getModelType() override;
	};
}