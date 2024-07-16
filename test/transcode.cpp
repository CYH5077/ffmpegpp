#include "gtest/gtest.h"

#include "config.hpp"

#include "ffmpegpp.hpp"

TEST(TRANS_CODE, COPY_PARAMETERS) {
    ff::ffmpegLogOff();

    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVVideoEncodeParametersPtr videoEncodeParameters = ff::FFAVVideoEncodeParameters::create(inputContext);
    ff::FFAVAudioEncodeParametersPtr audioEncodeParameters = ff::FFAVAudioEncodeParameters::create(inputContext);
    videoEncodeParameters->setEncodeThreadCount(16);

    ff::FFAVCodecContextPtr videoEncodeContext = ff::video::encode::createCodecContext(ff::VIDEO_CODEC::H264, videoEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext = ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("./transcode_copy.mp4");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.createStream(ff::DATA_TYPE::VIDEO, videoEncodeContext);
    outputContext.createStream(ff::DATA_TYPE::AUDIO, audioEncodeContext);
    error = outputContext.writeHeader();
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
        std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
    }
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVDecoder decoder(videoDecodeContext, audioDecodeContext);
    ff::FFAVEncoder encoder(videoEncodeContext, audioEncodeContext);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Decode
    error = decoder.decode(inputContext, [&](ff::FFAVFrame& frame) {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Encode
        error = encoder.encode(frame, [&](ff::FFAVPacket& packet) {
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
                return error;
            }

            return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
        });
        //////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode End
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode End
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}

TEST(TRANS_CODE, COPY_PARAMETERS_CUDA) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCUDACodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVVideoEncodeParametersPtr videoEncodeParameters = ff::FFAVVideoEncodeParameters::create(inputContext);
    ff::FFAVAudioEncodeParametersPtr audioEncodeParameters = ff::FFAVAudioEncodeParameters::create(inputContext);
    videoEncodeParameters->setEncodeThreadCount(16);

    ff::FFAVCodecContextPtr videoEncodeContext = ff::video::encode::createCUDACodecContext(ff::HW_VIDEO_CODEC::H264, videoEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext = ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("./transcode_copy_cuda.mp4");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.createStream(ff::DATA_TYPE::VIDEO, videoEncodeContext);
    outputContext.createStream(ff::DATA_TYPE::AUDIO, audioEncodeContext);
    error = outputContext.writeHeader();
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
        std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
    }
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVDecoder decoder(videoDecodeContext, audioDecodeContext);
    ff::FFAVEncoder encoder(videoEncodeContext, audioEncodeContext);

    // SwsContext - CUDA Format To YUV420P
    ff::FFAVSwsContext swsContext(videoEncodeContext->getWidth(), videoEncodeContext->getHeight(), ff::PICTURE_FORMAT::YUV420P);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Decode
    error = decoder.decode(inputContext, [&](ff::FFAVFrame& frame) {
        // CUDA Format to YUV420P
        if (frame.getType() == ff::DATA_TYPE::VIDEO) {
            swsContext.convert(frame);
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Encode
        error = encoder.encode(frame, [&](ff::FFAVPacket& packet) {
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
                return error;
            }

            return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
        });
        //////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode End
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode End
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}