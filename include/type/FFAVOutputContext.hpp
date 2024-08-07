#pragma once

#include <memory>
#include <vector>

#include "common/FFAVType.hpp"
#include "error/AVError.hpp"
#include "type/FFAVDecodeStream.hpp"
#include "type/FFAVEncodeStream.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVOutputContext {
    public:
        explicit FFAVOutputContext();
        virtual ~FFAVOutputContext();

    public:  // get set
        FFAVFormatContextImplPtr getImpl();

        AVError setOpt(const std::string& key, const std::string& value);

    public:
        AVError open(const std::string& filename);
        void close();

        FFAVEncodeStreamPtr addStream(VIDEO_CODEC videoCodec, FFAVDecodeStreamPtr decodeStream);
        FFAVEncodeStreamPtr addStream(AUDIO_CODEC audioCodec, FFAVDecodeStreamPtr decodeStream);

    private:
        FFAVEncodeStreamPtr createStream(FFAVEncodeStreamPtr encodeStream, FFAVDecodeStreamPtr decodeStream);
        FFAVCodecContextPtr createCodecContext(FFAVEncodeStreamPtr encodeStream, FFAVDecodeStreamPtr decodeStream);

    private:
        bool isWrite;
        FFAVFormatContextImplPtr formatContextImpl;

        FFAVEncodeStreamListPtr encodeStreamList;
    };
};
