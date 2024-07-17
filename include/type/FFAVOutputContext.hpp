#pragma once

#include <memory>

#include "error/AVError.hpp"
#include "type/ffav.hpp"

namespace ff {
    enum class OUTPUT_TYPE {
        HLS,
        RTMP,
        FILE,
    };

    static std::string OUTPUT_TYPE_TO_STRING(OUTPUT_TYPE type) {
        switch (type) {
            case OUTPUT_TYPE::HLS:
                return "hls";
            case OUTPUT_TYPE::RTMP:
                return "rtmp";
            case OUTPUT_TYPE::FILE:
                return "file";
        }

        return "none";
    }

    class FFAVOutputContext {
    public:
        explicit FFAVOutputContext();
        virtual ~FFAVOutputContext();

    public:
        AVError open(const std::string& filename, OUTPUT_TYPE type = OUTPUT_TYPE::FILE);
        void close();

        FFAVStreamPtr addStream(FFAVCodecParametersPtr codecParameters);

        AVError createStream(DATA_TYPE type, FFAVCodecContextPtr codecContext);

        AVError writeHeader();
        AVError writePacket(FFAVPacket& packet);

        AVError setOpt(const std::string& key, const std::string& value);

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
