#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

#include <memory>

namespace ff {
    typedef std::shared_ptr<AVFrame> AVFramePtr;

    class FFAVFrameImpl : public std::enable_shared_from_this<FFAVFrameImpl> {
    public:
        explicit FFAVFrameImpl();
        explicit FFAVFrameImpl(FFAVFrameImplPtr frameImpl);

        virtual ~FFAVFrameImpl();

    public: // 생성
        static FFAVFrameImplPtr create();
        static FFAVFrameImplPtr create(FFAVFrameImplPtr frameImpl);

    public:
        // Raw AVFrame
        AVFramePtr getRaw();
        void       setRaw(AVFramePtr frame);

        // AVFrame 복사
        FFAVFrameImplPtr copy();
        void             copyFrom(FFAVFrameImplPtr frame);
        void             copyFrom(AVFramePtr frame);

    private:
        AVFramePtr frame;
    };
};