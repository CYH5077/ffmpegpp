#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(TRANSCODE, TRANSCODE_GPU) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4", true); // GPU Decode
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto videoStreams = inputContext.getVideoDecodeStreamList();
    auto audioStreams = inputContext.getAudioDecodeStreamList();
    auto videoStream = videoStreams->at(0);
    auto audioStream = audioStreams->at(0);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("hw_h264_encode.m3u8");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.setOpt("hls_time", "3");
    outputContext.setOpt("hls_list_size", "0");
    outputContext.setOpt("hls_flags", "delete_segments+append_list+temp_file");
    outputContext.setOpt("hls_wrap", "0");

    auto encodeVideoStream = outputContext.addStream(ff::HW_VIDEO_CODEC::H264, videoStream); // GPU Encode
    auto encodeVideoStream2 = outputContext.addStream(ff::HW_VIDEO_CODEC::H264, videoStream); // GPU Encode
    auto encodeAudioStream = outputContext.addStream(ff::AUDIO_CODEC::AAC, audioStream);
    ASSERT_TRUE(encodeVideoStream != nullptr);
    ASSERT_TRUE(encodeVideoStream2 != nullptr);
    ASSERT_TRUE(encodeAudioStream != nullptr);

    encodeVideoStream2->setBitrate(9999);

    error = outputContext.writeHeader();
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVPacket packet;
    try {
        for (auto& packet : inputContext) {
            // Decode
            if (videoStream->getStreamIndex() == packet.getStreamIndex()) { // Video Stream Index
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

            } else if (audioStream->getStreamIndex() == packet.getStreamIndex()) { // Audio Stream Index
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

