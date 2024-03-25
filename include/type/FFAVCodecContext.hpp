#pragma once

#include "type/impl/ffavimpl.hpp"

#include <memory>

namespace ff {
    class FFAVCodecContext;
    typedef std::shared_ptr<FFAVCodecContext> FFAVCodecContextPtr;

    class FFAVCodecContext {
    public:
        explicit FFAVCodecContext();
        virtual ~FFAVCodecContext();

    public:
        static FFAVCodecContextPtr create();

    public:
        FFAVCodecContextImplPtr getImpl();

    private:
        FFAVCodecContextImplPtr codecContextImpl;
    };

}