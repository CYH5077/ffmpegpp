#include "gtest/gtest.h"

#include "config.hpp"

#include "ffmpegpp.hpp"

TEST(TRANS_CODE, COPY_PARAMETERS) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCodecContext(inputContext, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(inputContext, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVVideoEncodeParametersPtr videoEncodeParameters = ff::FFAVVideoEncodeParameters::create(inputContext);
    ff::FFAVAudioEncodeParametersPtr audioEncodeParameters = ff::FFAVAudioEncodeParameters::create(inputContext);
    videoEncodeParameters->setEncodeThreadCount(16);

    ff::FFAVCodecContextPtr videoEncodeContext = ff::video::encode::createCodecContext(ff::VIDEO_CODEC::H264, videoEncodeParameters, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext = ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("./transcode_copy.mp4");
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.createStream(ff::DATA_TYPE::VIDEO, videoEncodeContext);
    outputContext.createStream(ff::DATA_TYPE::AUDIO, audioEncodeContext);
    error = outputContext.writeHeader();
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
        std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
    }
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVDecoder decoder(videoDecodeContext, audioDecodeContext);
    ff::FFAVEncoder encoder(videoEncodeContext, audioEncodeContext);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Decode
    error = decoder.decode(inputContext, [&](ff::DATA_TYPE type, ff::FFAVFrame& frame) {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Encode
        error = encoder.encode(type, frame, [&](ff::FFAVPacket& packet) {
            if (packet.getType() == ff::DATA_TYPE::VIDEO) {
                packet.rescaleTS(inputContext.getVideoStream(), outputContext.getVideoStream());
                packet.setStreamIndex(inputContext.getVideoStreamIndex());
            } else if (packet.getType() == ff::DATA_TYPE::AUDIO) {
                packet.rescaleTS(inputContext.getAudioStream(), outputContext.getAudioStream());
                packet.setStreamIndex(inputContext.getAudioStreamIndex());
            }

            error = outputContext.writePacket(packet);
            if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
                std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
                return false;
            }

            return true;
        });
        //////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode End
        return true;
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode End
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}

TEST(TRANS_CODE, COPY_PARAMETERS_CUDA) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCodecContext(inputContext, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(inputContext, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVVideoEncodeParametersPtr videoEncodeParameters = ff::FFAVVideoEncodeParameters::create(inputContext);
    ff::FFAVAudioEncodeParametersPtr audioEncodeParameters = ff::FFAVAudioEncodeParameters::create(inputContext);
    videoEncodeParameters->setEncodeThreadCount(16);

    ff::FFAVCodecContextPtr videoEncodeContext = ff::video::encode::createCUDACodecContext(ff::HW_VIDEO_CODEC::H264, videoEncodeParameters, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext = ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("./transcode_copy_cuda.mp4");
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.createStream(ff::DATA_TYPE::VIDEO, videoEncodeContext);
    outputContext.createStream(ff::DATA_TYPE::AUDIO, audioEncodeContext);
    error = outputContext.writeHeader();
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
        std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
    }
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVDecoder decoder(videoDecodeContext, audioDecodeContext);
    ff::FFAVEncoder encoder(videoEncodeContext, audioEncodeContext);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Decode
    error = decoder.decode(inputContext, [&](ff::DATA_TYPE type, ff::FFAVFrame& frame) {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Encode
        error = encoder.encode(type, frame, [&](ff::FFAVPacket& packet) {
            if (packet.getType() == ff::DATA_TYPE::VIDEO) {
                packet.rescaleTS(inputContext.getVideoStream(), outputContext.getVideoStream());
                packet.setStreamIndex(inputContext.getVideoStreamIndex());
            } else if (packet.getType() == ff::DATA_TYPE::AUDIO) {
                packet.rescaleTS(inputContext.getAudioStream(), outputContext.getAudioStream());
                packet.setStreamIndex(inputContext.getAudioStreamIndex());
            }

            error = outputContext.writePacket(packet);
            if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
                std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
                return false;
            }

            return true;
        });
        //////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode End
        return true;
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode End
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}