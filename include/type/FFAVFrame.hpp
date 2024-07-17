#pragma once

#include <memory>

#include "type/FFAVDefine.hpp"
#include "type/impl/ffavimpl.hpp"

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
        void setType(DATA_TYPE type);
        
        
        DATA_TYPE getType();
        int getWidth();
        int getHeight();

    private:
        DATA_TYPE type;

        FFAVFrameImplPtr frameImpl;
    };
};
