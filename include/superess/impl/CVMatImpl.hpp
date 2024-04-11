#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "type/impl/FFAVFrameImpl.hpp"

#include "opencv2/opencv.hpp"

namespace ff::dnn {
    class CVMatImpl : public std::enable_shared_from_this<CVMatImpl> {
    public:
        explicit CVMatImpl();
        virtual ~CVMatImpl();

    public:
        static CVMatImplPtr create();

    public:
        cv::Mat getRaw();
        AVPixelFormat getAVFormat();

        FFAVFrameImplPtr toAVFrame();
        
        void setMat(cv::Mat image);
        void setAVFrame(FFAVFrameImplPtr frameImpl);
        void setAVFormat(AVPixelFormat format);


    private:
        cv::Mat cvMat;

        AVPixelFormat avFrameFormat;
    };
};