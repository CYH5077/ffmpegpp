#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "type/ffav.hpp"

namespace ff::dnn {
    class CVMat {
    public:
        explicit CVMat();
        virtual ~CVMat();

    public:
        int getAVFormat();

        FFAVFrame toFrame();

        void setMat(CVMat& mat);
        void setFrame(FFAVFrame& frame);
        void setAVFormat(int format);

    public:
        CVMatImplPtr getImpl();

    private:
        CVMatImplPtr cvMatImpl;
    };
};