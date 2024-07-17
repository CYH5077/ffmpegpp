#include "config.hpp"
#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

#include <filesystem>

static void transcode(const std::string& outputFilename,
                      int width,
                      int height,
                      ff::OUTPUT_TYPE type = ff::OUTPUT_TYPE::FILE);

TEST(STREAMING, HLS) {
    std::filesystem::create_directory("./hls");

    std::string outputFilename_1024_960 = "./hls/high_transcode_1024x960_.m3u8";
    std::string outputFilename_100_100 = "./hls/high_transcode_output100_.m3u8";

    transcode(outputFilename_1024_960, 1024, 960, ff::OUTPUT_TYPE::HLS);
    transcode(outputFilename_100_100, 100, 100, ff::OUTPUT_TYPE::HLS);
}

void transcode(const std::string& outputFilename, int width, int height, ff::OUTPUT_TYPE type) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVTranscoderParameter parameter(inputContext);

    error = parameter.createDecodeContext();
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoEncodeParameters = parameter.getVideoEncodeParameters();
    auto audioEncodeParameters = parameter.getAudioEncodeParameters();
    videoEncodeParameters->setEncodeThreadCount(16);
    videoEncodeParameters->setWidth(width);
    videoEncodeParameters->setHeight(height);

    error = parameter.createEncodeCodecContext(ff::VIDEO_CODEC::H264, ff::AUDIO_CODEC::AAC);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVTranscoder transcoder(parameter);
    if (type == ff::OUTPUT_TYPE::HLS) {
		transcoder.setOutputContextOpt("hls_time", "4");
		transcoder.setOutputContextOpt("hls_list_size", "3");
        transcoder.setOutputContextOpt("hls_flags", "delete_segments");
    } 

    transcoder.setErrorCallback([](ff::ERROR_TYPE type, ff::AVError& error) {
        std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
    });

    transcoder.transcode(outputFilename, type);
}
