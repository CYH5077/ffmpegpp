#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "superess/DnnSuperRes.hpp"

#include "error/ffav.hpp"

namespace ff::dnn {
    class EDSRSuperess : public DnnSuperRes {
    public:
        explicit EDSRSuperess();
        virtual ~EDSRSuperess();

    public:
        virtual SUPERESS_MODEL getModelType() override;
    };
};