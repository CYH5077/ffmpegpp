#pragma once

#include <memory>
#include <string>
#include <vector>

#include "error/AVError.hpp"
#include "type/FFAVChannelLayout.hpp"
#include "type/FFAVCodecParameters.hpp"
#include "type/FFAVDecodeStream.hpp"
#include "type/FFAVPacket.hpp"
#include "type/FFAVStream.hpp"

namespace ff {
    class FFAVInputContextIterator;

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
        FFAVDecodeStreamListPtr getVideoDecodeStreamList();
        FFAVDecodeStreamListPtr getAudioDecodeStreamList();

    public:  // for (auto& packet : inputContext) iterator
        FFAVInputContextIterator begin();
        FFAVInputContextIterator end();

    private:
        AVError parseStreamInfo(bool cudaDecode);

    private:
        FFAVFormatContextImplPtr formatContextImpl;
        FFAVDecodeStreamListPtr decodeStreamList;
    };

    // Iterator for FFAVInputContext
    class FFAVInputContextIterator {
    public:
        FFAVInputContextIterator(FFAVInputContext* context = nullptr);

        FFAVPacket& operator*();
        FFAVPacket& operator->();
        FFAVInputContextIterator& operator++();
        friend bool operator==(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b);
        friend bool operator!=(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b);

    private:
        bool eofFlag;

        FFAVInputContext* context;
        FFAVPacket currentPacket;
    };
};
