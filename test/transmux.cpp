#include "gtest/gtest.h"

#include "config.hpp"

#include "ffmpegpp.hpp"

TEST(TRANS_MUX, TEST_1) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    error = ff::transMux(inputContext, std::string("./trans_mux_test_1.mkv"));
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
        std::cout << error.getAVErrorMessage() << " " << error.getAVFunction() << std::endl;
    }
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
}

TEST(TRANS_MUX, TEST_2) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    std::string inputFilename = "./trans_mux_test_2.mkv";
    ASSERT_EQ(ff::transMux(inputContext, inputFilename).getType(), ff::AV_ERROR_TYPE::SUCCESS);
}

TEST(TRANS_MUX, TEST_3) {
    std::string inputFilename  = Config::SAMPLE_MP4;
    std::string outputFilename = "./trans_mux_test_3.mkv";
    ASSERT_EQ(ff::transMux(inputFilename, outputFilename).getType(), ff::AV_ERROR_TYPE::SUCCESS);
}