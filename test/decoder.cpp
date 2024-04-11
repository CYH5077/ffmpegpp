#include "gtest/gtest.h"

#include "config.hpp"
#include "ffmpegpp.hpp"

TEST(DECODE_TEST, DECODE_CPU) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVCodecContextPtr videoContext = ff::video::decode::createCodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioContext = ff::audio::decode::createCodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    int decodeCount = 0;
    ff::FFAVDecoder decoder(videoContext, audioContext);
    error = decoder.decode(inputContext, [&](ff::DATA_TYPE, ff::FFAVFrame& frame) {
        decodeCount++;
        return true;
    });
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    std::cout << "decodeCount: " << decodeCount << std::endl;
}

TEST(DECODE_TEST, DECODE_GPU) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVCodecContextPtr videoContext = ff::video::decode::createCUDACodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    ff::FFAVCodecContextPtr audioContext = ff::audio::decode::createCodecContext(inputContext, &error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    int decodeCount = 0;
    ff::FFAVDecoder decoder(videoContext, audioContext);
    error = decoder.decode(inputContext, [&](ff::DATA_TYPE, ff::FFAVFrame& frame) {
        decodeCount++;
        return true;
    });
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    std::cout << "decodeCount: " << decodeCount << std::endl;
}