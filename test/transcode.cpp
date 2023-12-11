#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "Muxer.hpp"
#include "CodecContext.hpp"
#include "Encoder.hpp"
#include "Decoder.hpp"

TEST(TRANS_CODE, TRANS_CODE) {
    av::AVResult result;

    // Demuxer
    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    av::CodecContextPtr decodeVideoCodecContext = av::createVideoDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr decodeAudioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());


    const av::Stream& demuxerVideoStream = demuxer.getVideoStream();
    av::VideoEncodeParameter videoEncodeParameter;
    videoEncodeParameter.setBitrate(500000);
    videoEncodeParameter.setWidth(demuxer.getWidth());
    videoEncodeParameter.setHeight(demuxer.getHeight());
    videoEncodeParameter.setTimeBase(demuxerVideoStream.getTimebase());
    videoEncodeParameter.setFrameRate(demuxerVideoStream.getFramerate());
    videoEncodeParameter.setGOPSize(10);
    videoEncodeParameter.setMaxBFrames(0);
    videoEncodeParameter.setPixelFormat(av::PIXEL_FORMAT::YUV420P);
    videoEncodeParameter.setThreadCount(10);

    av::CodecContextPtr encodeAudioCodecContext = nullptr;
    av::CodecContextPtr encodeVideoCodecContext = av::createVideoEncodeContext(av::CODEC_ID::H264, videoEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.open(TEST::TRANSCODING_MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.createNewStream(encodeVideoCodecContext, &result);
    ASSERT_TRUE(result.isSuccess());
    // 디코딩된 패킷을 그냥 쓰기때문에 디코딩시 사용된 AVCodecContext를 그냥 사용해서 작성
    muxer.createNewStream(decodeAudioCodecContext, &result); 
    ASSERT_TRUE(result.isSuccess());
    muxer.writeHeader(&result);
    ASSERT_TRUE(result.isSuccess());


    av::Encoder encoder(encodeVideoCodecContext, encodeAudioCodecContext);
    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);
    decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame) {
        if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
            encoder.encode(packet.getMediaType(), decodeFrame, [&](av::Packet& encodePacket){
                encodePacket.rescaleTS(demuxerVideoStream.getTimebase(), muxer.getTimebase());
                muxer.writePacket(encodePacket, &result);
                ASSERT_TRUE(result.isSuccess());
            }, &result);
            ASSERT_TRUE(result.isSuccess());
        } else if (packet.getMediaType() == av::MEDIA_TYPE::AUDIO) {
            muxer.writePacket(packet, &result);
            ASSERT_TRUE(result.isSuccess());
        }
    }, &result);   

    encoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
}