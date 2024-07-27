#include "gtest/gtest.h"

#include "config.hpp"

#include "ffmpegpp.hpp"

TEST(RESIZE_TRANS_CODE, VIDEO_DOWN_SIZE) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoStreams();
    auto audioStreams = inputContext.getAudioStreams();
    auto videoStream = videoStreams->size() > 0 ? (*videoStreams)[0] : nullptr;
    auto audioStream = audioStreams->size() > 0 ? (*audioStreams)[0] : nullptr;

    ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCUDACodecContext(videoStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(audioStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVVideoEncodeParametersPtr videoEncodeParameters = ff::FFAVVideoEncodeParameters::create(videoStream);
    ff::FFAVAudioEncodeParametersPtr audioEncodeParameters = ff::FFAVAudioEncodeParameters::create(audioStream);
    videoEncodeParameters->setEncodeThreadCount(16);
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Resize the video
    videoEncodeParameters->setWidth(1280);
    videoEncodeParameters->setHeight(640);

    ff::FFAVCodecContextPtr videoEncodeContext = ff::video::encode::createCUDACodecContext(ff::HW_VIDEO_CODEC::H264, videoEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext = ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("./transcode_down_size_640_480.mp4");
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

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Resize SwsContext
    ff::FFAVSwsContext swsContext(1280, 640, ff::PICTURE_FORMAT::YUV420P);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Decode
    error = decoder.decode(inputContext, [&](ff::FFAVFrame& frame) {
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Resize Frame
        if (frame.getType() == ff::DATA_TYPE::VIDEO) {
            swsContext.convert(frame);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///Encode
        error = encoder.encode(frame, [&](ff::FFAVPacket& packet) {
            error = outputContext.writePacket(packet);
            if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
                std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
                return error;
            }

            return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
        });
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///Encode End
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            return error;
        }
        
        return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode End
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}
