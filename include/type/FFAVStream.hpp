#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVStream {
    public:
        explicit FFAVStream();
        virtual ~FFAVStream();

    public:
         FFAVStreamImplPtr getImpl();

    private:
        FFAVStreamImplPtr streamImpl;
    };
};