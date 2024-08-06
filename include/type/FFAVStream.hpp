#pragma once

#include <memory>
#include <vector>

#include "type/FFAVChannelLayout.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVStream;
    using FFAVStreamPtr = std::shared_ptr<FFAVStream>;

    class FFAVStream {
    public:
        explicit FFAVStream(DATA_TYPE type);
        virtual ~FFAVStream();

    public:
        static FFAVStreamPtr create(DATA_TYPE type);

    public:
        FFAVStreamImplPtr getImpl();

    private:
        FFAVStreamImplPtr streamImpl;
    };

    using FFAVStreamList = std::vector<FFAVStreamPtr>;
    using FFAVStreamListPtr = std::shared_ptr<FFAVStreamList>;
};
