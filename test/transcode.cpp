#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

#include <filesystem>

void transcode_test(const std::string& outputFileName, ff::HW_VIDEO_CODEC hwVideoCodec, ff::VIDEO_CODEC videoCodec);

TEST(TRANSCODE, TRANSCODE_H264_H265_MP4) {
    std::filesystem::create_directory("./transcode");
    transcode_test("./transcode/01_h264.mp4", ff::HW_VIDEO_CODEC::H264, ff::VIDEO_CODEC::H264);
    transcode_test("./transcode/02_h265.mp4", ff::HW_VIDEO_CODEC::H265, ff::VIDEO_CODEC::H265);
}

TEST(TRANSCODE, HLS_TEST_H264_H265) {
    std::filesystem::create_directory("./hls_h264");
    transcode_test("./hls_h264/03_h264.m3u8", ff::HW_VIDEO_CODEC::H264, ff::VIDEO_CODEC::H264);

    std::filesystem::create_directory("./hls_h265");
    transcode_test("./hls_h265/04_h265.m3u8", ff::HW_VIDEO_CODEC::H265, ff::VIDEO_CODEC::H265);
}

TEST(TRANSCODE, DASH_TEST_H264_H265) {
    std::filesystem::create_directory("./dash_h264");
    transcode_test("./dash_h264/05_h264.mpd", ff::HW_VIDEO_CODEC::H264, ff::VIDEO_CODEC::H264);

    std::filesystem::create_directory("./dash_h265");
	transcode_test("./dash_h265/06_h265.mpd", ff::HW_VIDEO_CODEC::H265, ff::VIDEO_CODEC::H265);
}

void transcode_test(const std::string& outputFileName, ff::HW_VIDEO_CODEC hwVideoCodec, ff::VIDEO_CODEC videoCodec) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4", true);  // GPU Decode
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoDecodeStreamList();
    auto audioStreams = inputContext.getAudioDecodeStreamList();
    auto videoStream = videoStreams->at(0);
    auto audioStream = audioStreams->at(0);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open(outputFileName);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    if (outputFileName.find(".m3u8") != std::string::npos) {
        outputContext.setOpt("hls_time", "3");
        outputContext.setOpt("hls_list_size", "0");
        outputContext.setOpt("hls_flags", "delete_segments+append_list");
    }

    auto encodeVideoStream = outputContext.addStream(hwVideoCodec, videoStream); // GPU Encode
    auto encodeVideoStream2 = outputContext.addStream(videoCodec, videoStream);   // GPU Encode
    auto encodeAudioStream = outputContext.addStream(ff::AUDIO_CODEC::AAC, audioStream);
    ASSERT_TRUE(encodeVideoStream != nullptr);
    ASSERT_TRUE(encodeVideoStream2 != nullptr);
    ASSERT_TRUE(encodeAudioStream != nullptr);
    
    // Configure Video Stream
    encodeVideoStream2->setBitrate(9999);

    ASSERT_TRUE(encodeVideoStream->openCodec().getType() == ff::AV_ERROR_TYPE::SUCCESS);
    ASSERT_TRUE(encodeVideoStream2->openCodec().getType() == ff::AV_ERROR_TYPE::SUCCESS);
    ASSERT_TRUE(encodeAudioStream->openCodec().getType() == ff::AV_ERROR_TYPE::SUCCESS);

    error = outputContext.writeHeader();
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVPacket packet;
    try {
        for (auto& packet : inputContext) {
            // Decode
            if (videoStream->getStreamIndex() == packet.getStreamIndex()) {  // Video Stream Index
                auto videoFrameList = videoStream->decode(packet, &error);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

                auto videoPacketList = encodeVideoStream->encode(videoFrameList, &error);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
                error = outputContext.writePacket(videoPacketList);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

                auto videoPacketList2 = encodeVideoStream2->encode(videoFrameList, &error);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
                error = outputContext.writePacket(videoPacketList2);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

            } else if (audioStream->getStreamIndex() == packet.getStreamIndex()) {  // Audio Stream Index
                auto audioFrameList = audioStream->decode(packet, &error);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

                auto audioPacketList = encodeAudioStream->encode(audioFrameList, &error);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

                error = outputContext.writePacket(audioPacketList);
                ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
            }
        }
    } catch (ff::AVDemuxException& e) {
        ff::AVError error = e.getAVError();
        std::cout << "libav error message: " << error.getAVErrorMessage() << std::endl;
        std::cout << "error message: " << error.getMessage() << std::endl;
    }

    auto videoFrameList = encodeVideoStream->flush(&error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    auto videoFrameList2 = encodeVideoStream2->flush(&error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    auto audioFrameList = encodeAudioStream->flush(&error);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    error = outputContext.writePacket(videoFrameList);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    error = outputContext.writePacket(videoFrameList2);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    error = outputContext.writePacket(audioFrameList);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.close();
}
