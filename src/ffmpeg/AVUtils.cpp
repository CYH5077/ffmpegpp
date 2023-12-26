#include "ffmpeg/AVUtils.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/hwcontext.h"
}

namespace av {
    bool isCudaVideoEncodingDecodingAvailable() {
        if (av_hwdevice_iterate_types(AV_HWDEVICE_TYPE_CUDA) == AV_HWDEVICE_TYPE_NONE) {
            return false;
        }
        return true;
    }

    void getCudaVideoDecoderList(HWDecoderList* hwDecoderList) {
        if (isCudaVideoEncodingDecodingAvailable() == false ||
            hwDecoderList                          == nullptr) {
            return;
        }

        const AVCodec* avCodec = nullptr;
        void* iter = nullptr;
        while ((avCodec = av_codec_iterate(&iter))) {
            if (avCodec->type == AVMEDIA_TYPE_VIDEO && av_codec_is_decoder(avCodec)) {
                const AVCodecHWConfig* codecHWConfig = nullptr;
                for (int i = 0; (codecHWConfig = avcodec_get_hw_config(avCodec, i)); i++) {
                    if (codecHWConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                        codecHWConfig->device_type == AV_HWDEVICE_TYPE_CUDA) {
                        hwDecoderList->emplace_back(avCodec->name);
                    }
                }
            }
        }
    }

    void getCudaVideoEncoderList(HWEncoderList* hwEncoderList) {
        if (isCudaVideoEncodingDecodingAvailable() == false ||
            hwEncoderList                          == nullptr) {
            return;
        }

        const AVCodec* avCodec = nullptr;
        void* iter = nullptr;
        while ((avCodec = av_codec_iterate(&iter))) {
            if (avCodec->type == AVMEDIA_TYPE_VIDEO && av_codec_is_encoder(avCodec)) {
                const AVCodecHWConfig* codecHwConfig = nullptr;
                for (int i = 0; (codecHwConfig = avcodec_get_hw_config(avCodec, i)); i++) {
                    if (codecHwConfig->device_type == AV_HWDEVICE_TYPE_CUDA) {
                        hwEncoderList->emplace_back(avCodec->name);
                    }
                }
            }
        }
    }
};