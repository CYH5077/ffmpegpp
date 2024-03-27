#pragma once

#include "type/ffav.hpp"

#include "error/AVError.hpp"

#include <memory>

namespace ff {
    class FFAVOutputContext {
    public:
        explicit FFAVOutputContext();
        virtual ~FFAVOutputContext();

    public:
        AVError open(std::string& filename);
        AVError open(std::string&& filename);
        void close();

        FFAVStreamPtr addStream(FFAVCodecParametersPtr codecParameters);

        AVError createStream(DATA_TYPE type, FFAVCodecContextPtr codecContext);

        AVError writeHeader();
        AVError writePacket(FFAVPacket& packet);

    public:
        FFAVFormatContextImplPtr getImpl();

        FFAVStreamPtr getVideoStream();
        FFAVStreamPtr getAudioStream();
        FFAVStreamPtr getStream(int index);

    private:
        bool isOpenFlag;

        FFAVFormatContextImplPtr formatContextImpl;

        FFAVStreamPtr videoStream;
        FFAVStreamPtr audioStream;
    };
};