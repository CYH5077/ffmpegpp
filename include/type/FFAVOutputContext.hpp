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

        AVError writeHeader();
        AVError writePacket(FFAVPacket packet);
        AVError writePacket(FFAVPacketListPtr packetList);

        FFAVEncodeStreamPtr addStream(HW_VIDEO_CODEC videoCodec, FFAVDecodeStreamPtr decodeStream);
        FFAVEncodeStreamPtr addStream(VIDEO_CODEC videoCodec, FFAVDecodeStreamPtr decodeStream);
        FFAVEncodeStreamPtr addStream(AUDIO_CODEC audioCodec, FFAVDecodeStreamPtr decodeStream);
        AVError copyStreams(FFAVDecodeStreamListPtr streamList);

    private:
        FFAVEncodeStreamPtr createStream(FFAVEncodeStreamPtr encodeStream, FFAVDecodeStreamPtr decodeStream, bool isVideo);
        FFAVCodecContextPtr createCodecContext(FFAVEncodeStreamPtr encodeStream, FFAVDecodeStreamPtr decodeStream, bool isVideo);

    private:
        FFAVFormatContextImplPtr formatContextImpl;

        std::vector<std::pair<FFAVDecodeStreamPtr, FFAVEncodeStreamPtr>> streamList;
    };
};
