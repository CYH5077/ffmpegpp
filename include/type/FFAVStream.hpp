#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVStream {
    public:
        explicit FFAVStream();
        explicit FFAVStream(FFAVStream& stream);
        explicit FFAVStream(FFAVStreamImplPtr streamImpl);

        virtual ~FFAVStream();

    public:
         FFAVStreamImplPtr getImpl();

    private:
        FFAVStreamImplPtr streamImpl;
    };
};