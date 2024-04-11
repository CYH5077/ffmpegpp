#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "superess/CVMat.hpp"

#include "error/ffav.hpp"

namespace ff::dnn {
    enum class EDSR_SCALE {
        X2 = 2,
        X3 = 3,
        X4 = 4
    };

    class EDSRSuperess {
    public:
        explicit EDSRSuperess();
        virtual ~EDSRSuperess();

    public:
        AVError openModel(const std::string& modelPath, EDSR_SCALE scale);
        void    enableCuda(bool isEnable);

        void upsample(CVMat& srcImage, CVMat* dstImage);
        void upsample(FFAVFrame& srcFrame, FFAVFrame* dstFrame);

    private:
        DnnSuperResImplPtr dnnSuperResImpl;
    };
};