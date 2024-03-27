#include "codecs/createDecodeContext.hpp"

#include "type/impl/FFAVCodecContextImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"


namespace ff {
    FFAVCodecContextPtr createDecodeCodecContext(FFAVCodecParametersPtr ffavCodecParameters, AVError* error, bool cudaEnable) {
        if (error == nullptr || ffavCodecParameters == nullptr) {
            *error = AVError(AV_ERROR_TYPE::SUCCESS);
            return nullptr;
        }

        FFAVCodecContextPtr ffavCodecContext = FFAVCodecContext::create();

        AVCodecParameters* codecParameters = ffavCodecParameters->getImpl()->getRaw();
        if (codecParameters == nullptr) {
            error->setError(AV_ERROR_TYPE::FAILED, "codecParameters is nullptr", -1 , "");
            return nullptr;
        }

        const AVCodec* decodeCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (decodeCodec == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_decoder failed", AVERROR_DECODER_NOT_FOUND, "avcodec_find_decoder");
            return nullptr;
        }

        AVCodecContext* decodeCodecContext = avcodec_alloc_context3(decodeCodec);
        if (decodeCodecContext == nullptr) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_alloc_context3 failed", AVERROR(ENOMEM), "avcodec_alloc_context3");
            return nullptr;
        }
        ffavCodecContext->getImpl()->setRaw(decodeCodecContext);

        int ret = 0;
        if (cudaEnable == true) {
            ret = av_hwdevice_ctx_create(&decodeCodecContext->hw_device_ctx, AV_HWDEVICE_TYPE_CUDA, nullptr, nullptr, 0);
            if (ret < 0) {
                error->setError(AV_ERROR_TYPE::AV_ERROR, "av_hwdevice_ctx_create failed", ret, "av_hwdevice_ctx_create");
                return nullptr;
            }

            if (ffavCodecContext->findCUDAHWFormat() == false) {
                error->setError(AV_ERROR_TYPE::AV_ERROR, "findCUDAHWFormat failed", AVERROR_DECODER_NOT_FOUND, "findCUDAHWFormat");
                return nullptr;
            }
        }

        ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
        if (ret < 0) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_parameters_to_context failed", ret, "avcodec_parameters_to_context");
            return nullptr;
        }

        ret = avcodec_open2(decodeCodecContext, decodeCodec, nullptr);
        if (ret < 0) {
            error->setError(AV_ERROR_TYPE::AV_ERROR, "avcodec_open2 failed", ret, "avcodec_open2");
            return nullptr;
        }


        ffavCodecContext->setEnableCuda(cudaEnable);
        *error = AVError(AV_ERROR_TYPE::SUCCESS);
        return ffavCodecContext;
    }
};

namespace ff::video::decode {
    FFAVCodecContextPtr createCodecContext(FFAVInputContext& inputContext, AVError* error) {
        return createDecodeCodecContext(inputContext.getVideoCodecParameters(), error, false);
    }

    FFAVCodecContextPtr createCUDACodecContext(FFAVInputContext& inputContext, AVError* error) {
        return createDecodeCodecContext(inputContext.getVideoCodecParameters(), error, true);
    }
};

namespace ff::audio::decode {
    FFAVCodecContextPtr createCodecContext(FFAVInputContext& inputContext, AVError* error) {
        return createDecodeCodecContext(inputContext.getAudioCodecParameters(), error, false);
    }
};

