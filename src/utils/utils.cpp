#include "utils/utils.hpp"

#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"

extern "C" {
#include "libavutil/log.h"
}

namespace ff {

    void ffmpegLogOff() {
        av_log_set_level(AV_LOG_QUIET);
    }

    AVError copyStream(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>* streamMapper) {
        int streamIndex = 0;

        if (streamMapper != nullptr) {
            streamMapper->resize(inputContext.getStreamsCount(), 0);
        }

        for (unsigned int i = 0; i < inputContext.getStreamsCount(); i++) {
            AVCodecParameters* inputCodecParameters = inputContext.getCodecParameters(i)->getImpl()->getRaw();
            if (inputCodecParameters->codec_type != AVMEDIA_TYPE_VIDEO &&
                inputCodecParameters->codec_type != AVMEDIA_TYPE_AUDIO &&
                inputCodecParameters->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                if (streamMapper != nullptr) {
                    (*streamMapper)[i] = -1;
                }
                continue;
            }

            if (streamMapper != nullptr) {
                (*streamMapper)[i] = streamIndex++;
            }

            FFAVStreamPtr ffavStream = outputContext->addStream(inputContext.getCodecParameters(i));
            if (ffavStream == nullptr) {
                return AVError(AV_ERROR_TYPE::AV_ERROR, "memory alloc failed");
            }
            ffavStream->getImpl()->getRaw()->codecpar->codec_tag = 0;
        }
        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError copyPackets(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>& streamMapper) {
        FFAVPacket ffavPacket;
       while (inputContext.readFrame(&ffavPacket).getType() != AV_ERROR_TYPE::AV_EOF) {
           //AVPacket* packet = iter.getImpl()->getRaw().get();
            AVPacket* packet = ffavPacket.getImpl()->getRaw().get();
            if (packet->stream_index >= streamMapper.size() || streamMapper[packet->stream_index] == -1) {
                av_packet_unref(packet);
                continue;
            }

            AVStream* inputStream  = inputContext.getImpl()->getRaw()->streams[packet->stream_index];
            packet->stream_index = streamMapper[packet->stream_index];
            AVStream* outputStream = outputContext->getImpl()->getRaw()->streams[packet->stream_index];
            av_packet_rescale_ts(packet, inputStream->time_base, outputStream->time_base);
            packet->pos = -1;

            AVError error = outputContext->writePacket(ffavPacket);
            if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                return error;
            }
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};