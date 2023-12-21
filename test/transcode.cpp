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


    const av::CodecParameters demuxerVideoCodecParameters = demuxer.getVideoCodecParameters();
    const av::Stream& demuxerVideoStream = demuxer.getVideoStream();
    av::VideoEncodeParameters videoEncodeParameter;
    videoEncodeParameter.setBitrate(demuxerVideoCodecParameters.getBitrate());
    videoEncodeParameter.setWidth(demuxerVideoCodecParameters.getWidth());
    videoEncodeParameter.setHeight(demuxerVideoCodecParameters.getHeight());
    videoEncodeParameter.setTimeBase(demuxerVideoStream.getTimebase());
    videoEncodeParameter.setFrameRate(demuxerVideoStream.getFramerate());
    videoEncodeParameter.setGOPSize(150);
    videoEncodeParameter.setMaxBFrames(0);
    videoEncodeParameter.setPixelFormat(av::PIXEL_FORMAT::YUV420P);
    videoEncodeParameter.setThreadCount(10);

    const av::CodecParameters demuxerAudioCodecParameters = demuxer.getAudioCodecParameters();
    const av::Stream& demuxerAudioStream = demuxer.getAudioStream();
    av::AudioEncodeParameters audioEncodeParameter;
    audioEncodeParameter.setBitrate(demuxerAudioCodecParameters.getBitrate());
    audioEncodeParameter.setTimebase(demuxerAudioStream.getTimebase());
    audioEncodeParameter.setSampleFormat(av::SAMPLE_FORMAT::FLTP);
    audioEncodeParameter.setSamplerate(demuxerAudioCodecParameters.getSamplerate());
    audioEncodeParameter.setChannelLayout(demuxerAudioCodecParameters.getChannelLayout());

    av::CodecContextPtr encodeVideoCodecContext = av::createVideoEncodeContext(av::CODEC_ID::H264, videoEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr encodeAudioCodecContext = av::createAudioEncodeContext(av::CODEC_ID::AAC, audioEncodeParameter, &result);
    //av::CodecContextPtr  encodeAudioCodecContext = nullptr;
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.open(TEST::TRANSCODING_MP4_FILE, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.createNewStream(encodeVideoCodecContext, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.createNewStream(encodeAudioCodecContext, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.writeHeader(&result);
    ASSERT_TRUE(result.isSuccess());

    const av::Stream& encodeVideoStream = muxer.getVideoStream();
    const av::Stream& encodeAudioStream = muxer.getAudioStream();

    av::Encoder encoder(encodeVideoCodecContext, encodeAudioCodecContext);
    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);
    decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame) {
        encoder.encode(packet.getMediaType(), decodeFrame, [&](av::Packet &encodePacket) {
            if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
                encodePacket.rescaleTS(demuxerVideoStream.getTimebase(), encodeVideoStream.getTimebase());
                encodePacket.setStreamIndex(demuxer.getVideoStreamIndex());
            } else if (packet.getMediaType() == av::MEDIA_TYPE::AUDIO) {
                encodePacket.rescaleTS(demuxerAudioStream.getTimebase(), encodeAudioStream.getTimebase());
                encodePacket.setStreamIndex(demuxer.getAudioStreamIndex());
            }
            muxer.writePacket(encodePacket, &result);
            //ASSERT_TRUE(result.isSuccess());
        }, &result);
        //ASSERT_TRUE(result.isSuccess());
    }, &result);
    encoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
}