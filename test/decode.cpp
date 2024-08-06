#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(READ_TEST, AV_PACKET_READ) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4", true);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoDecodeStreamList();
    auto audioStreams = inputContext.getAudioDecodeStreamList();
    auto videoStream = videoStreams->at(0);
    auto audioStream = audioStreams->at(0);

    int readCount = 0;
    int decodeCount = 0;

    ff::FFAVPacket packet;
    while (true) {
        // Demux
        error = inputContext.demux(&packet);
        if (error.getType() != ff::AV_ERROR_TYPE::AV_EOF) {
            ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
            readCount++;
        }

        // Decoder flush
        if (error.getType() == ff::AV_ERROR_TYPE::AV_EOF) {
            auto videoFrameList = videoStream->decodeFlush();
            auto audioFrameList = audioStream->decodeFlush();

            decodeCount += videoFrameList->size() + audioFrameList->size();

            break;
        }

        // Decode
        if (videoStream->getStreamIndex() == packet.getStreamIndex()) {
            auto videoFrameList = videoStream->decode(packet, &error);

            decodeCount += videoFrameList->size();
        } else if (audioStream->getStreamIndex() == packet.getStreamIndex()) {
            auto audioFrameList = audioStream->decode(packet, &error);

            decodeCount += audioFrameList->size();
        }
    }

    std::cout << "Read Count: " << readCount << std::endl;
    std::cout << "Decode Count: " << decodeCount << std::endl;
}
