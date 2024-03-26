#pragma once

#include "type/FFAVPacket.hpp"

#include "type/FFAVCodecParameters.hpp"
#include "type/FFAVStream.hpp"

#include "error/AVError.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ff {
    class FFAVInputContext;

    class FFAVInputContextIterator {
    public:
        FFAVInputContextIterator(FFAVInputContext* context = nullptr);

        FFAVPacket& operator*();
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

        int getStreamsCount();

        FFAVFormatContextImplPtr getImpl();
        FFAVCodecParametersPtr getVideoCodecParameters();
        FFAVCodecParametersPtr getAudioCodecParameters();
        FFAVCodecParametersPtr getCodecParameters(int index);
        FFAVStreamPtr getVideoStream();
        FFAVStreamPtr getAudioStream();
        FFAVStreamPtr getStream(int index);
        int getVideoStreamIndex();
        int getAudioStreamIndex();

    public: // for (auto& packet : inputContext) iterator
        FFAVInputContextIterator begin();
        FFAVInputContextIterator end();

    private:
        void findMetaData();

    private:
        bool isOpenedFlag = false;

        FFAVFormatContextImplPtr formatContextImpl;

        // CodecParameters
        std::vector<FFAVCodecParametersPtr> codecParameters;

        // Streams
        std::vector<FFAVStreamPtr> streams;

        // Stream Index
        int videoStreamIndex;
        int audioStreamIndex;
    };

};