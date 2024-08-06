#pragma once

#include <memory>
#include <vector>

#include "type/FFAVChannelLayout.hpp"
#include "type/FFAVCodecContext.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {

    template <typename T>
    class FFAVStream {
    public:
        explicit FFAVStream(DATA_TYPE type) {
            this->type = type;
            this->streamIndex = -1;
        }

        virtual ~FFAVStream() {}

    public:
        static std::shared_ptr<T> create(DATA_TYPE type) {
            return std::make_shared<T>(type);
        }

    public:
        FFAVStreamImplPtr getImpl() {
            return this->streamImpl;
        }

        DATA_TYPE getType() {
            return this->type;
        }

        void setType(DATA_TYPE dataType) {
            this->type = dataType;
        }

        int getStreamIndex() {
            return this->streamIndex;
        }

        void setStreamIndex(int streamIndex) {
            this->streamIndex = streamIndex;
        }

        FFAVCodecContextPtr getCodecContext() {
            return this->codecContext;
        }

        void setCodecContext(FFAVCodecContextPtr codecContext) {
            this->codecContext = codecContext;
        }

    protected:
        DATA_TYPE type;

        int streamIndex;

        FFAVStreamImplPtr streamImpl;
        FFAVCodecContextPtr codecContext;
    };
};
