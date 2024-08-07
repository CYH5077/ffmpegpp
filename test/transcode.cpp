#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(TRANSCODE, H265_CPU) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4", true);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoDecodeStreamList();
    auto audioStreams = inputContext.getAudioDecodeStreamList();
    auto videoStream = videoStreams->at(0);
    auto audioStream = audioStreams->at(0);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("output.mp4");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto encodeVideoStream = outputContext.addStream(ff::VIDEO_CODEC::H265, videoStream);
    auto encodeAudioStream = outputContext.addStream(ff::AUDIO_CODEC::AAC, audioStream);
    ASSERT_TRUE(encodeVideoStream != nullptr);
    ASSERT_TRUE(encodeAudioStream != nullptr);

    ff::FFAVPacket packet;
    try {
        for (auto& packet : inputContext) {
            // Decode
            if (videoStream->getStreamIndex() == packet.getStreamIndex()) {
                auto videoFrameList = videoStream->decode(packet, &error);

            } else if (audioStream->getStreamIndex() == packet.getStreamIndex()) {
                auto audioFrameList = audioStream->decode(packet, &error);

            }
        }
    } catch (ff::AVDemuxException& e) {
        ff::AVError error = e.getAVError();
        std::cout << "libav error message: " << error.getAVErrorMessage() << std::endl;
        std::cout << "error message: " << error.getMessage() << std::endl;
    }

}
