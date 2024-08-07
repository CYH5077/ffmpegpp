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
        void setCodec(VIDEO_CODEC codec);
        void setCodec(AUDIO_CODEC codec);

        bool isVideoStream();
        bool isAudioStream();

        VIDEO_CODEC getVideoCodec();
        AUDIO_CODEC getAudioCodec();

    public:
        AVError encode();

    private:
        VIDEO_CODEC videoCodec;
        AUDIO_CODEC audioCodec;
    };

    using FFAVEncodeStreamList = std::vector<FFAVEncodeStreamPtr>;
    using FFAVEncodeStreamListPtr = std::shared_ptr<FFAVEncodeStreamList>;
}
