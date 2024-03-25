#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVCodecParameters {
    public:
        explicit FFAVCodecParameters();
        explicit FFAVCodecParameters(FFAVCodecParameters& codecParameters);
        explicit FFAVCodecParameters(FFAVCodecParametersImplPtr codecParametersImpl);

        virtual ~FFAVCodecParameters();

    public:
        FFAVCodecParametersImplPtr getImpl();

    private:
        FFAVCodecParametersImplPtr codecParametersImpl;
    };
};