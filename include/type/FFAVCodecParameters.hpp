#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVCodecParameters {
    public:
        explicit FFAVCodecParameters();
        virtual ~FFAVCodecParameters();

    public:
        FFAVCodecParametersImplPtr getImpl();

    private:
        FFAVCodecParametersImplPtr codecParametersImpl;
    };
};