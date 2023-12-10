#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVType.hpp"
#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "CodecContext.hpp"
#include "Decoder.hpp"
#include "Packet.hpp"
#include "Frame.hpp"
#include "Muxer.hpp"
#include "VideoEncodeParameter.hpp"
#include "Encoder.hpp"

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
        //std::cout << packet.getPTS() << " "
        //          << packet.getDTS() << " "
        //          << packet.getSize() << std::endl;
        packet.unref();
    }

    demuxer.close();
}

static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, const char *filename)
{
    FILE *f;
    int i;
    f = fopen(filename,"w");
    // writing the minimal required header for a pgm file format
    // portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    // writing line by line
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

TEST(Muxer, Muxer_Mux) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.transMux(demuxer, TEST::MKV_FILE, &result);
    ASSERT_TRUE(result.isSuccess());
}

TEST(Encoder, Encoder_encode) {
    av::AVResult result;

    // Demuxer
    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    auto decodeVideoCodecContext = av::createVideoDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    auto decodeAudioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());

    av::VideoEncodeParameter videoEncodeParameter;
    videoEncodeParameter.setBitrate(500000);
    videoEncodeParameter.setWidth(demuxer.getWidth());
    videoEncodeParameter.setHeight(demuxer.getHeight());
    videoEncodeParameter.setTimeBase(demuxer.getTimebase());
    videoEncodeParameter.setFrameRate(demuxer.getFrameRate());
    videoEncodeParameter.setGOPSize(10);
    videoEncodeParameter.setMaxBFrames(0);
    videoEncodeParameter.setPixelFormat(av::PIXEL_FORMAT::YUV420P);
    videoEncodeParameter.setThreadCount(10);

    av::CodecContextPtr encodeAudioCodecContext = nullptr;
    av::CodecContextPtr encodeVideoCodecContext = av::createVideoEncodeContext(av::CODEC_ID::H264, videoEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.open(TEST::OUTPUT_VIDEO_TRANSCODING_MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.createNewStream(encodeVideoCodecContext, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.writeHeader(&result);
    ASSERT_TRUE(result.isSuccess());


    av::Encoder encoder(encodeVideoCodecContext, encodeAudioCodecContext);
    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);
    decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame) {
        if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
            encoder.encode(packet.getMediaType(), decodeFrame, [&](av::Packet& encodePacket){
                encodePacket.rescalePTS(demuxer.getTimebase(), muxer.getTimebase());
                muxer.writePacket(encodePacket, &result);
                ASSERT_TRUE(result.isSuccess());
            }, &result);
        }
    }, &result);   

    encoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
}