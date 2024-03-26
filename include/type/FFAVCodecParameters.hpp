#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVCodecParameters;

    typedef std::shared_ptr<FFAVCodecParameters> FFAVCodecParametersPtr;

    class FFAVCodecParameters {
    public:
        explicit FFAVCodecParameters();
        virtual ~FFAVCodecParameters();

    public:
        static FFAVCodecParametersPtr create();

    public:
        FFAVCodecParametersImplPtr getImpl();

    private:
        FFAVCodecParametersImplPtr codecParametersImpl;
    };
};