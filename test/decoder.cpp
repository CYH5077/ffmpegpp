#include "gtest/gtest.h"

#include "config.hpp"
#include "ffmpegpp.hpp"

TEST(DECODE_TEST, DECODE_CPU) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoStreams();
    auto audioStreams = inputContext.getAudioStreams();
    auto videoStream = videoStreams->size() > 0 ? (*videoStreams)[0] : nullptr;
    auto audioStream = audioStreams->size() > 0 ? (*audioStreams)[0] : nullptr;
    
    ff::FFAVCodecContextPtr videoContext = ff::video::decode::createCodecContext(videoStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioContext = ff::audio::decode::createCodecContext(audioStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    int decodeCount = 0;
    ff::FFAVDecoder decoder(videoContext, audioContext);
    error = decoder.decode(inputContext, [&](ff::FFAVFrame& frame) {
        decodeCount++;
        return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
    });
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    std::cout << "decodeCount: " << decodeCount << std::endl;
}

TEST(DECODE_TEST, DECODE_GPU) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoStreams();
    auto audioStreams = inputContext.getAudioStreams();
    auto videoStream = videoStreams->size() > 0 ? (*videoStreams)[0] : nullptr;
    auto audioStream = audioStreams->size() > 0 ? (*audioStreams)[0] : nullptr;
    
    ff::FFAVCodecContextPtr videoContext = ff::video::decode::createCUDACodecContext(videoStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioContext = ff::audio::decode::createCodecContext(audioStream, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    int decodeCount = 0;
    ff::FFAVDecoder decoder(videoContext, audioContext);
    error = decoder.decode(inputContext, [&](ff::FFAVFrame& frame) {
        decodeCount++;
        return ff::AVError(ff::AV_ERROR_TYPE::SUCCESS);
    });
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    std::cout << "decodeCount: " << decodeCount << std::endl;
}