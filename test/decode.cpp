#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(READ_TEST, AV_PACKET_READ_ITERATOR) {
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
    try {
        for (auto& packet : inputContext) {
            readCount++;

            // Decode
            if (videoStream->getStreamIndex() == packet.getStreamIndex()) {
                auto videoFrameList = videoStream->decode(packet, &error);
                decodeCount += videoFrameList->size();
            } else if (audioStream->getStreamIndex() == packet.getStreamIndex()) {
                auto audioFrameList = audioStream->decode(packet, &error);
                decodeCount += audioFrameList->size();
            }
        }
    } catch (ff::AVDemuxException& e) {
        ff::AVError error = e.getAVError();
        std::cout << "libav error message: " << error.getAVErrorMessage() << std::endl;
        std::cout << "error message: " << error.getMessage() << std::endl;
    }

    std::cout << "Read Count: " << readCount << std::endl;
    std::cout << "Decode Count: " << decodeCount << std::endl;
}

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
        } else {  // Decoder Flush
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
