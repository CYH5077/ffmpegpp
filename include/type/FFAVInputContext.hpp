#pragma once

#include "type/FFAVPacket.hpp"
#include "type/FFAVCodecParameters.hpp"
#include "type/FFAVStream.hpp"

#include "error/AVError.hpp"

#include <memory>
#include <string>

namespace ff {
    class FFAVInputContext;

    class FFAVInputContextIterator {
    public:
        FFAVInputContextIterator(FFAVInputContext* context = nullptr);

        const FFAVPacket& operator*() const;
        FFAVPacket* operator->();
        FFAVInputContextIterator& operator++();
        friend bool operator==(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b);
        friend bool operator!=(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b);

    private:
        FFAVInputContext* context;
        FFAVPacket currentPacket;
    };

    class FFAVInputContext {
    public:
        explicit FFAVInputContext();
        virtual ~FFAVInputContext();

    public:
        AVError open(const std::string& url);
        AVError open(const std::string&& url);
        void close();

        AVError readFrame(FFAVPacket* packet);

    public: // get set
        bool isOpened() const;

        FFAVFormatContextImplPtr getImpl();

    public: // for (auto& packet : inputContext) iterator
        FFAVInputContextIterator begin();
        FFAVInputContextIterator end();

    private:
        void findMetaData();

    private:
        bool isOpenedFlag = false;

        FFAVFormatContextImplPtr formatContextImpl;

        // CodecParameters
        FFAVCodecParameters videoCodecParameters;
        FFAVCodecParameters audioCodecParameters;

        // Stream
        FFAVStream videoStream;
        FFAVStream audioStream;
    };
};