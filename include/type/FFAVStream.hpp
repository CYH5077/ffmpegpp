#pragma once

#include <memory>
#include <vector>

#include "type/FFAVChannelLayout.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVStream;

    typedef std::shared_ptr<FFAVStream> FFAVStreamPtr;

    class FFAVStream {
    public:
        explicit FFAVStream(DATA_TYPE type);
        virtual ~FFAVStream();

    public:
        static FFAVStreamPtr create(DATA_TYPE type);

    public:
        FFAVStreamImplPtr getImpl();
        int getStreamIndex();
        DATA_TYPE getDataType();
        FFAVChannelLayoutPtr getChannelLayout();

        void setStreamIndex(int streamIndex);
        void setDataType(DATA_TYPE dataType);

    private:
        DATA_TYPE dataType;

        int streamIndex;

        FFAVStreamImplPtr streamImpl;
    };

    using FFAVStreamList = std::vector<FFAVStreamPtr>;
    using FFAVStreamListPtr = std::shared_ptr<FFAVStreamList>;
};
