#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "error/AVError.hpp"

#include <memory>

#include "opencv2/opencv.hpp"
#include "opencv2/dnn_superres.hpp"

namespace ff::dnn {
    class DnnSuperResImpl : public std::enable_shared_from_this<DnnSuperResImpl> {
    public:
        explicit DnnSuperResImpl();
        virtual ~DnnSuperResImpl();

    public:
        static DnnSuperResImplPtr create();

    public:
        AVError readModel(const std::string& modelPath);

        // Cuda 활성화
        void enableCuda(bool isEnable);

        // Model 설정 (EDSR, ESPCN, Fsrcnn, LapSRN, VESPCN)
        void setModel(const std::string& model, int scale);

        void upsample(CVMatImplPtr srcImage, CVMatImplPtr dstImage);

    private:
        cv::dnn_superres::DnnSuperResImpl dnnSuperResImpl;

        int scale;
    };
};