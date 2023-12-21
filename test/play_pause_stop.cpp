#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "Decoder.hpp"

#include <thread>
#include <chrono>

TEST(PLAY_PAUSE_TEST, PLAY_PAUSE_TEST) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::CodecContextPtr decodeVideoCodecContext = av::createVideoDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr decodeAudioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    av::AVResult decodeResult;
    int decodeCount = 0;
    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);
    std::thread thread([&](){
        decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* ) {
            std::cout << "PLAY_PAUSE_TEST Decode count - " << decodeCount++ << std::endl;
        }, &decodeResult);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    decoder.pause();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    decoder.play();

    thread.join();
    ASSERT_TRUE(decodeResult.isSuccess());
}

TEST(PLAY_STOP_TEST, PLAY_STOP_TEST) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::CodecContextPtr decodeVideoCodecContext = av::createVideoDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr decodeAudioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    int packetCount = demuxer.getPacketCount(&result);
    ASSERT_TRUE(result.isSuccess());

    int decodeCount = 0;
    av::AVResult decodeResult;
    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);
    std::thread thread([&](){
        decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* ) {
            decodeCount++;
        }, &decodeResult);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    decoder.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    decoder.play();

    std::cout << "PLAY_STOP_TEST totalCount : " << packetCount << std::endl;
    std::cout << "PLAY_STOP_TEST decodeCount: " << decodeCount << std::endl;

    thread.join();
    ASSERT_TRUE(packetCount > decodeCount);
    ASSERT_TRUE(decodeResult.isSuccess());
}