#include "util/transMux.hpp"

#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVFrameImpl.hpp"

#include <iostream>
#include <vector>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    AVError copyStream(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>* streamMapper);
    AVError copyPackets(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>& streamMapper);

    AVError transMux(FFAVInputContext& inputContext, const std::string& outputFilename) {
        std::vector<int> streamMapper;
        FFAVOutputContext outputContext;

        AVError error = outputContext.open(outputFilename);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        error = copyStream(inputContext, &outputContext, &streamMapper);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        error = outputContext.writeHeader();
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        error = copyPackets(inputContext, &outputContext, streamMapper);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError transMux(FFAVInputContext& inputContext, const std::string&& outputFilename) {
        return transMux(inputContext, outputFilename);
    }

    AVError transMux(const std::string& inputFilename , const std::string& outputFilename) {
        FFAVInputContext inputContext;
        AVError error = inputContext.open(inputFilename);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return transMux(inputContext, outputFilename);
    }

    AVError copyStream(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>* streamMapper) {
        int streamIndex = 0;
        streamMapper->resize(inputContext.getStreamsCount(), 0);
        for (unsigned int i = 0; i < inputContext.getStreamsCount(); i++) {
            AVCodecParameters* inputCodecParameters = inputContext.getCodecParameters(i)->getImpl()->getRaw();
            if (inputCodecParameters->codec_type != AVMEDIA_TYPE_VIDEO &&
                inputCodecParameters->codec_type != AVMEDIA_TYPE_AUDIO &&
                inputCodecParameters->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                (*streamMapper)[i] = -1;
                continue;
            }

            (*streamMapper)[i] = streamIndex++;

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
       // for (auto& iter : inputContext) {
       int i = 0;
       while (inputContext.readFrame(&ffavPacket).getType() != AV_ERROR_TYPE::AV_EOF) {
           i++;
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
                std::cout << i << std::endl;
                return error;
            }
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};
