#pragma once

#include "superess/impl/ffavimpl.hpp"

#include "superess/CVMat.hpp"

#include "error/ffav.hpp"

namespace ff::dnn {
    enum class SUPERESS_MODEL {
        EDSR,
        ESPCN,
        FSRCNN,
    };

    static const std::string SUPERESS_MODEL_TO_STRING(SUPERESS_MODEL model) {
        switch (model) {
        case SUPERESS_MODEL::EDSR:   return "edsr";
        case SUPERESS_MODEL::ESPCN:  return "espcn";
        case SUPERESS_MODEL::FSRCNN: return "fsrcnn";
        }
    }

    enum class SUPERESS_SCALE {
        X2 = 2,
        X3 = 3,
        X4 = 4
    };

    class DnnSuperRes {
    public:
        explicit DnnSuperRes();
        virtual ~DnnSuperRes();

    public:
        virtual SUPERESS_MODEL getModelType() = 0;

    public:
        AVError openModel(const std::string& modelPath, SUPERESS_SCALE scale);
        void    enableCuda(bool isEnable);

        void upsample(CVMat& srcImage, CVMat* dstImage);
        void upsample(FFAVFrame& srcFrame, FFAVFrame* dstFrame);

    private:
        DnnSuperResImplPtr dnnSuperResImpl;
    };
}