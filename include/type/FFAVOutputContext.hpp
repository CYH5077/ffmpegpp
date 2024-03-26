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

        AVError writeHeader();
        AVError writePacket(FFAVPacket& packet);

    public:
        FFAVFormatContextImplPtr getImpl();

        FFAVStreamPtr getStream(int index);

    private:
        bool isOpenFlag;

        FFAVFormatContextImplPtr formatContextImpl;
    };
};