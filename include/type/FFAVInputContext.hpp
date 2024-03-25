#pragma once

#include "type/FFAVPacket.hpp"

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

    public: // for (auto& packet : inputContext)
        FFAVInputContextIterator begin();
        FFAVInputContextIterator end();

    public: // get set
        bool isOpened() const;

        FFAVFormatContextImplPtr getImpl();

    private:
        bool isOpenedFlag = false;

        FFAVFormatContextImplPtr formatContextImpl;
    };
};