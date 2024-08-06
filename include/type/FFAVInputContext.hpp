#pragma once

#include <memory>
#include <string>
#include <vector>

#include "error/AVError.hpp"
#include "type/FFAVChannelLayout.hpp"
#include "type/FFAVCodecParameters.hpp"
#include "type/FFAVPacket.hpp"
#include "type/FFAVStream.hpp"
#include "type/FFAVDecodeStream.hpp"

namespace ff {
    class FFAVInputContext {
    public:
        explicit FFAVInputContext();
        virtual ~FFAVInputContext();

    public:  // get set
        FFAVFormatContextImplPtr getImpl();

    public:
        AVError open(const std::string& url, bool cudaDecode = false);
        bool isOpened();
        void close();

        AVError demux(FFAVPacket* packet);

        FFAVDecodeStreamListPtr getDecodeStreamList();

    private:
        AVError parseStreamInfo(bool cudaDecode);

    private:
        FFAVFormatContextImplPtr formatContextImpl;
        FFAVDecodeStreamListPtr decodeStreamList;
    };

};
