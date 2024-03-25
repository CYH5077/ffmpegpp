#include "gtest/gtest.h"

#include "ffmpegpp.hpp"

TEST(TYPE_TEST, INPUT_CONTEXT_OPEN_1) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open("./sample.mp4");
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    inputContext.close();
}

TEST(TYPE_TEST, INPUT_CONTEXT_DESTROY_TEST_1) {
    ff::FFAVInputContext inputContext;
}

TEST(TYPE_TEST, INPUT_CONTEXT_DESTROY_TEST_2) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open("./sample.mp4");
    EXPECT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
}