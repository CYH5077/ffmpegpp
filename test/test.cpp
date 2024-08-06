#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(READ_TEST, AV_PACKET_READ) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto streams = inputContext.getDecodeStreamList();

    ff::FFAVPacket packet;
    while (true) {
        error = inputContext.demux(&packet);
        if (error.getType() == ff::AV_ERROR_TYPE::AV_EOF) {
            break;
        }
        ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);


        // decode
        for (auto iter : *streams) {
            if (iter->getStreamIndex() == packet.getStreamIndex()) {
                auto frameList = iter->decode(packet, &error);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
            }
        }
    }

    if (error.getType() == ff::AV_ERROR_TYPE::AV_EOF) {
        for (auto iter : *streams) {
            auto frameList = iter->decodeFlush();
        }
    }
}
