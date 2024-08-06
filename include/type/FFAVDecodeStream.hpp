#pragma once

#include <memory>
#include <vector>

#include "type/FFAVStream.hpp"
#include "type/FFAVPacket.hpp"
#include "type/FFAVFrame.hpp"

namespace ff {
    class FFAVDecodeStream;
    using FFAVDecodeStreamPtr = std::shared_ptr<FFAVDecodeStream>;

    class FFAVDecodeStream : public FFAVStream<FFAVDecodeStream> {
    public:
        explicit FFAVDecodeStream(DATA_TYPE type);

        virtual ~FFAVDecodeStream();

    public:
        FFAVFrameListPtr decode(FFAVPacket& packet, AVError* error = nullptr);
        FFAVFrameListPtr decodeFlush();
    };

    using FFAVDecodeStreamList = std::vector<FFAVDecodeStreamPtr>;
    using FFAVDecodeStreamListPtr = std::shared_ptr<FFAVDecodeStreamList>;
};
