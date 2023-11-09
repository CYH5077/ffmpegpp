#include "gtest/gtest.h"

#include "FFMPEG.hpp"

TEST(TEST1, TEST_CASE) {
    ASSERT_EQ(1, 1);
}

TEST(TEST2, TEST_CASE2) {
    ASSERT_NE(1, 2);
}

TEST(TEST1, TEST_VERSION_INFO) {
    FFMPEG ffmpeg;
    ffmpeg.printVersion();   
    ASSERT_EQ(1, 1);
}