#pragma once

#include <memory>

#include "type/FFAVDefine.hpp"
#include "type/impl/ffavimpl.hpp"
#include "type/FFAVStream.hpp"

namespace ff {
    class FFAVFrame {
    public:
        explicit FFAVFrame();
        explicit FFAVFrame(FFAVFrameImplPtr frameImpl);

        virtual ~FFAVFrame();

    public:
        FFAVFrameImplPtr getImpl();

    public:
        void copyTs(FFAVFrame& frame);
        void ref(FFAVFrame& frame);

    public:
        DATA_TYPE getType();
        int getWidth();
        int getHeight();

        void setType(DATA_TYPE type);

    private:
        DATA_TYPE type;

        FFAVFrameImplPtr frameImpl;
    };
};
