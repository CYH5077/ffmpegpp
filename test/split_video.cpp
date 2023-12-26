#include "gtest/gtest.h"

#include <string>

#include "TEST_DEFINE.hpp"

#include "ffmpegpp.hpp"

/*
static void openAndWriteHeader(av::Muxer& muxer, const std::string& fileName, av::Demuxer& demuxer, av::AVResult* result) {
    av::CodecContextPtr videoCodecContext = av::createVideoDecodeContext(demuxer, result);
    if (result->isSuccess() == false) {
        return ;
    }

    muxer.open(fileName, result);
    if (result->isSuccess() == false) {
        return;
    }

    muxer.createNewStream(videoCodecContext, result);
    if (result->isSuccess() == false) {
        return;
    }

    muxer.writeHeader(result);
    if (result->isSuccess() == false) {
        return;
    }
}
*/

// Split 2 videos
TEST(SPLIT_VIDEO, SPLIT_VIDEO) {
    /*
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer1;
    av::Muxer muxer2;
    openAndWriteHeader(muxer1, TEST::SPLIT_1_MP4_FILE, demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    openAndWriteHeader(muxer2, TEST::SPLIT_2_MP4_FILE, demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Packet packet;
    while (demuxer.read(&packet, &result)) {
        if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
            const av::Stream& videoStream = demuxer.getVideoStream();
            double ptsTime = packet.getPTSTimeToSecond(videoStream.getTimebase());

        }
    }
    */
}