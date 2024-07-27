#include "gtest/gtest.h"

#include "ffmpegpp.hpp"
#include "config.hpp"

TEST(STREAMING_SEGMENT, HLS_TEST) {
    // ff::ffmpegLogOff();

    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoStreams();
    auto audioStreams = inputContext.getAudioStreams();
    auto videoStream = videoStreams->size() > 0 ? (*videoStreams)[0] : nullptr;
    auto audioStream = audioStreams->size() > 0 ? (*audioStreams)[0] : nullptr;

    std::cout << "total frame count: " << inputContext.getFrameCount() << std::endl;

    ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCodecContext(videoStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(audioStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVVideoEncodeParametersPtr videoEncodeParameters = ff::FFAVVideoEncodeParameters::create(videoStream);
    ff::FFAVAudioEncodeParametersPtr audioEncodeParameters = ff::FFAVAudioEncodeParameters::create(audioStream);
    videoEncodeParameters->setEncodeThreadCount(16);

    ff::FFAVCodecContextPtr videoEncodeContext =
        ff::video::encode::createCodecContext(ff::VIDEO_CODEC::H264, videoEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioEncodeContext =
        ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameters, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////  Output Hls
    ff::FFAVOutputContext outputContext;
    error = outputContext.open("./hls.m3u8", ff::OUTPUT_TYPE::HLS);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    outputContext.setOpt("hls_time", "3");
    outputContext.setOpt("hls_list_size", "0");
    outputContext.setOpt("hls_flags", "delete_segments");
    outputContext.setOpt("hls_segment_filename", "hls_%03d.ts");

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
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Encode
        error = encoder.encode(frame, [&](ff::FFAVPacket& packet) {
            error = outputContext.writePacket(packet);
            if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
                std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
                return error;
            }

            return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
        });
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Encode End
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
    });
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode
    /// End
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    encoder.flush();
}
