#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "ffmpegpp.hpp"

#include <thread>
#include <vector>

TEST(CUDA, CUDA_DECODE_SINGLE) {
    ASSERT_TRUE(av::isCudaVideoEncodingDecodingAvailable());

    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::CodecContextPtr videoCodecContext = av::createVideoCUDADecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr audioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    int decodeFrameCount = 0;
    av::Decoder decoder(videoCodecContext, audioCodecContext);
    decoder.decode(demuxer, [&](av::Packet &packet, av::Frame &decodeFrame, av::AVResult *decodeResult) {
        decodeFrameCount++;
    }, &result);
    ASSERT_TRUE(result.isSuccess());

    std::cout << "decode frame count: " << decodeFrameCount << std::endl;
}