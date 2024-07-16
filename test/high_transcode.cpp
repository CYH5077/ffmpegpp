#include "config.hpp"
#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(HIGH_TRNASCODE, TRANSCODE_COPY) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVTranscoderParameter parameter(inputContext);

    error = parameter.createDecodeContext();
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoEncodeParameters = parameter.getVideoEncodeParameters();
    auto audioEncodeParameters = parameter.getAudioEncodeParameters();
    videoEncodeParameters->setEncodeThreadCount(16);

    error = parameter.createEncodeCodecContext(ff::VIDEO_CODEC::H264, ff::AUDIO_CODEC::AAC);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVTranscoder transcoder(parameter);
    transcoder.setErrorCallback([](ff::ERROR_TYPE type, ff::AVError& error) {
		std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
	});

    transcoder.transcode("high_transcode.mp4");
}
