#pragma once

#include <memory>
#include <string>
#include <vector>

#include "error/AVError.hpp"
#include "type/FFAVChannelLayout.hpp"
#include "type/FFAVCodecParameters.hpp"
#include "type/FFAVPacket.hpp"
#include "type/FFAVStream.hpp"

namespace ff {
    class FFAVInputContext;

    class FFAVInputContextIterator {
    public:
        FFAVInputContextIterator(FFAVInputContext* context = nullptr);

        FFAVPacket& operator*();
        FFAVPacket& operator->();
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

        int getFrameCount();

    public:  // get set
        bool isOpened() const;

        int getStreamsCount();

        FFAVFormatContextImplPtr getImpl();

        FFAVCodecParametersPtr getCodecParameters(int index);

        FFAVStreamListPtr getStreams();
        FFAVStreamListPtr getVideoStreams();
        FFAVStreamListPtr getAudioStreams();

        AVError setOpt(const std::string& key, const std::string& value);

    public:  // for (auto& packet : inputContext) iterator
        FFAVInputContextIterator begin();
        FFAVInputContextIterator end();

    private:
        void findMetaData();

    private:
        bool isOpenedFlag = false;

        FFAVFormatContextImplPtr formatContextImpl;

        // Streams
        FFAVStreamListPtr videoStreams;
        FFAVStreamListPtr audioStreams;
        FFAVStreamListPtr streams; // all streams
    };

};
