#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "CodecContext.hpp"
#include "Decoder.hpp"

TEST(DECODE, DECODE_1) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::CodecContextPtr videoCodecContext = av::createVideoDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr audioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    int decodeFrameCount = 0;
    av::Decoder decoder(videoCodecContext, audioCodecContext);
    decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* decodeResult){
        decodeFrameCount++;
    }, &result);
    ASSERT_TRUE(result.isSuccess());

    std::cout << "decode frame count: " << decodeFrameCount << std::endl;
}

TEST(DECODE, DECODE_2) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::CodecContextPtr videoCodecContext = av::createVideoDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr audioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Decoder decoder(videoCodecContext, audioCodecContext);

    int decodeFrameCount = 0;
    av::Packet packet;
    while (demuxer.read(&packet, &result)) {
        decoder.decode(packet, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* decodeResult){
                decodeFrameCount++;
                decodeResult->success();
        }, &result);
        ASSERT_TRUE(result.isSuccess());
    }
    ASSERT_TRUE(result.isSuccess());

    decoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());

    std::cout << "deocde frame count: " << decodeFrameCount << std::endl;
}