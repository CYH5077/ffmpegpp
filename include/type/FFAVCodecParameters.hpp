#pragma once

#include <memory>
#include <vector>

#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVCodecParameters;
    using FFAVCodecParametersPtr = std::shared_ptr<FFAVCodecParameters>;

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

    using FFAVCodecParametersList = std::vector<FFAVCodecParametersPtr>;
};
