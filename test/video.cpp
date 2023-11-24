#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "CodecContext.hpp"
#include "Decoder.hpp"
#include "Packet.hpp"
#include "Frame.hpp"
#include "Muxer.hpp"
#include "EncodeParameter.hpp"

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

TEST(CodecContext, CodecContext_CreateDecodeContext) {
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

TEST(Decoder, Decoder_Decode) {
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

    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);
    decoder.decode(demuxer, [&](AVMediaType type, av::Frame& frame) {
        if (type == AVMEDIA_TYPE_VIDEO) {
            //AVFrame* avframe = frame.getRawFrame();
            //save_gray_frame(avframe->data[0], avframe->linesize[0], avframe->width, avframe->height, "test.pgm");
            //frame.printDump();
        } else if (type == AVMEDIA_TYPE_AUDIO) {
            //std::cout << "Audio frame!!" << std::endl;
        }
    }, &result);
    ASSERT_TRUE(result.isSuccess());
}

TEST(Muxer, Muxer_Mux) {
    av::AVResult result;

    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.mux(demuxer, TEST::MKV_FILE, &result);
    ASSERT_TRUE(result.isSuccess());
}

TEST(Encoder, Encoder_encode) {
    av::AVResult result;

    av::EncodeParameter encodeParameter;
    encodeParameter.setBitrate(400000);
    encodeParameter.setWidth(1900);
    encodeParameter.setHeight(1680);
    encodeParameter.setTimeBase(av::Rational(1, 25));
    encodeParameter.setFrameRate(av::Rational(25, 1));
    encodeParameter.setGOPSize(10);
    encodeParameter.setMaxBFrames(0);
    encodeParameter.setPixelFormat(AV_PIX_FMT_YUV420P);

    av::CodecContext codecContext;
    av::createEncodeContext(AV_CODEC_ID_H264, encodeParameter, &codecContext, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.open(TEST::OUTPUT_MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());

    muxer.createNewStream(codecContext, &result);
    ASSERT_TRUE(result.isSuccess());

    muxer.writeHeader(&result);
    ASSERT_TRUE(result.isSuccess());
}