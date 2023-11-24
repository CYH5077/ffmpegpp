#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "CodecContext.hpp"

TEST(Demuxer, Demuxer_Open) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    demuxer.printDump();

    demuxer.close();
}

TEST(Demuxer, Demuxer_ReadPacket) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Packet packet;
    while (demuxer.read(&packet, &result)) {
        std::cout << packet.getPTS() << " "
                  << packet.getDTS() << " "
                  << packet.getSize() << std::endl;
    }

    demuxer.close();
}

TEST(CodecContext, CodecContext_createDecodeContext) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::CodecContext decodeVideoCodecContext;
    if (demuxer.isVideoCodecParameters()) {
        av::createVideoDecodeContext(demuxer, &decodeVideoCodecContext, &result);
        ASSERT_TRUE(result.isSuccess());
    }

    av::CodecContext decodeAudioCodecContext;
    if (demuxer.isAudioCodecParameters()) {
        av::createAudioDecodeContext(demuxer, &decodeAudioCodecContext, &result);
        ASSERT_TRUE(result.isSuccess());
    }
    
}