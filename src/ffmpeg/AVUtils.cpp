#include "ffmpeg/AVUtils.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/hwcontext.h"
#include "libavutil/time.h"
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

    bool convertFrameSizeWithToRGB(Frame& targetFrame, Frame* convertFrame, int width, int height, AVResult* result) {
        if (result       == nullptr ||
            convertFrame == nullptr) {
            return false;
        }

        AVFrame* targetRawFrame  = targetFrame.getRawFrame();
        AVFrame* convertRawFrame = convertFrame->getRawFrame();

        SwsContext* swsContext = sws_getContext(targetRawFrame->width, targetRawFrame->height, (AVPixelFormat)targetRawFrame->format,
            								    width,                 height,                  AV_PIX_FMT_RGB24,
            									SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (swsContext == nullptr) {
            return result->failed(-1, "sws_context create failed");
        }

        convertRawFrame->format = AV_PIX_FMT_RGB24;
        convertRawFrame->width  = width;
        convertRawFrame->height = height;
        av_frame_get_buffer(convertRawFrame, 32);

        sws_scale(swsContext, targetRawFrame->data, targetRawFrame->linesize, 0, targetRawFrame->height, convertRawFrame->data, convertRawFrame->linesize);
        sws_freeContext(swsContext);
        return result->success();
    }

    long long getLocalTimeMicroseconds() {
        return av_gettime_relative();
    }

    long long getPTSTimeToMicroseconds(long long pts, Rational& timebase) {
        /*
        double timebaseValue = av_q2d(AVRational{timebase.getNum(), timebase.getDen()});
        double ptsTime       = pts * timebaseValue;
        */
        return av_rescale_q(pts, AVRational{timebase.getNum(), timebase.getDen()}, AV_TIME_BASE_Q);
	}
};