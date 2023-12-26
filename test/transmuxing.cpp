#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "ffmpegpp.hpp"


TEST(TRANS_MUXING, TRANS_MUXING) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.transMux(demuxer, TEST::TRNAS_MUXING_MKV_FILE, &result);
    ASSERT_TRUE(result.isSuccess());
}