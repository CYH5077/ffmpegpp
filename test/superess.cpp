#include "gtest/gtest.h"

#include "config.hpp"

#include "ffmpegpp.hpp"
#include "ffmpegpp_dnn.hpp"

void transcodeSuperess(const std::string& outputFile, const std::string& modelFile, ff::dnn::EDSR_SCALE scale, bool isCuda);
TEST(SUPERESS, X2) {
    transcodeSuperess("edsr_x2_cuda.ts", Config::EDSR_X2_MODEL_FILE, ff::dnn::EDSR_SCALE::X2, true);
}

TEST(SUPERESS, X3) {
    transcodeSuperess("edsr_x3_cuda.ts", Config::EDSR_X3_MODEL_FILE, ff::dnn::EDSR_SCALE::X3, true);
}

TEST(SUPERESS, X4) {
    transcodeSuperess("edsr_x4_cuda.ts", Config::EDSR_X4_MODEL_FILE, ff::dnn::EDSR_SCALE::X4, true);
}

void transcodeSuperess(const std::string& outputFile, const std::string& modelFile, ff::dnn::EDSR_SCALE scale, bool isCuda) {
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
    videoEncodeParameters->setGOPSize(2);

    // bitrate  * (scale ^ 2)
    // width * scale
    // heght * scale
    videoEncodeParameters->setBitrate(videoEncodeParameters->getBitrate() * ((int)scale * (int)scale));
    videoEncodeParameters->setWidth(videoEncodeParameters->getWidth()   * (int)scale);
    videoEncodeParameters->setHeight(videoEncodeParameters->getHeight() * (int)scale);

    ff::FFAVCodecContextPtr videoEncodeContext = ff::video::encode::createCUDACodecContext(ff::HW_VIDEO_CODEC::H264, videoEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext = ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open(outputFile);
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

    // EDSR Model
    ff::dnn::EDSRSuperess superess;
    superess.openModel(modelFile, scale);
    superess.enableCuda(isCuda);

    int count = 0;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Decode
    error = decoder.decode(inputContext, [&](ff::DATA_TYPE type, ff::FFAVFrame& frame) {
        if (type == ff::DATA_TYPE::VIDEO) {
            //////////////////////////////////////////////////////////////////////////////////////////////////////////// EDSR
            ff::FFAVFrame superessFrame;
            superess.upsample(frame, &superessFrame);
            frame.ref(superessFrame);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Encode
        error = encoder.encode(type, frame, [&](ff::FFAVPacket& packet) {
            if (packet.getType() == ff::DATA_TYPE::VIDEO) {
                packet.rescaleTS(inputContext.getVideoStream(), outputContext.getVideoStream());
                packet.setStreamIndex(inputContext.getVideoStreamIndex());
            }
            else if (packet.getType() == ff::DATA_TYPE::AUDIO) {
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

        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
            return false;
        }
        return true;
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode End
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}