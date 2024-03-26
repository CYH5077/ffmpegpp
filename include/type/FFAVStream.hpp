#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVStream;

    typedef std::shared_ptr<FFAVStream> FFAVStreamPtr;

    class FFAVStream {
    public:
        explicit FFAVStream();
        virtual ~FFAVStream();

    public:
        static FFAVStreamPtr create();

    public:
         FFAVStreamImplPtr getImpl();

    private:
        FFAVStreamImplPtr streamImpl;
    };
};