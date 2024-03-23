#include "gtest/gtest.h"

#include "ffmpegpp.hpp"

TEST(TEST1, TEST1) {
    auto formatContext = ff::FFAVFormatContext::create();
    auto packet = ff::FFAVPacket::create();
}