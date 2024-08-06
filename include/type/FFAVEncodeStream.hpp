#pragma once

#include <memory>
#include <vector>

#include "type/FFAVStream.hpp"

namespace ff {
    class FFAVEncodeStream;
    using FFAVEncodeStreamPtr = std::shared_ptr<FFAVEncodeStream>;

    class FFAVEncodeStream : public FFAVStream<FFAVEncodeStream> {
    public:
        explicit FFAVEncodeStream(DATA_TYPE type);

        virtual ~FFAVEncodeStream();

    public:
        AVError encode();
    };

    using FFAVEncodeStreamList = std::vector<FFAVEncodeStreamPtr>;
    using FFAVEncodeStreamListPtr = std::shared_ptr<FFAVEncodeStreamList>;
}
