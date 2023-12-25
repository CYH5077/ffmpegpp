#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "Demuxer.hpp"
#include "Muxer.hpp"
#include "CodecContext.hpp"
#include "Encoder.hpp"
#include "Decoder.hpp"

TEST(TRANS_CODE, TRANS_CODE_1) {
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
    videoEncodeParameter.setEncodeThreadCount(10);

    const av::CodecParameters demuxerAudioCodecParameters = demuxer.getAudioCodecParameters();
    const av::Stream& demuxerAudioStream = demuxer.getAudioStream();
    av::AudioEncodeParameters audioEncodeParameter;
    audioEncodeParameter.setBitrate(demuxerAudioCodecParameters.getBitrate());
    audioEncodeParameter.setTimebase(demuxerAudioStream.getTimebase());
    audioEncodeParameter.setSampleFormat(av::SAMPLE_FORMAT::FLTP);
    audioEncodeParameter.setSamplerate(demuxerAudioCodecParameters.getSamplerate());
    audioEncodeParameter.setChannelLayout(demuxerAudioCodecParameters.getChannelLayout());

    av::CodecContextPtr encodeVideoCodecContext = av::createVideoEncodeContext(av::VIDEO_CODEC_ID::H264, videoEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr encodeAudioCodecContext = av::createAudioEncodeContext(av::AUDIO_CODEC_ID::AAC, audioEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.open(TEST::TRANSCODING_MP4_FILE_1, &result);
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode
    decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* decodeReuslt) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode
        encoder.encode(packet.getMediaType(), decodeFrame, [&](av::Packet &encodePacket, av::AVResult* encodeResult) {
            if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
                encodePacket.rescaleTS(demuxerVideoStream.getTimebase(), encodeVideoStream.getTimebase());
                encodePacket.setStreamIndex(demuxer.getVideoStreamIndex());
            } else if (packet.getMediaType() == av::MEDIA_TYPE::AUDIO) {
                encodePacket.rescaleTS(demuxerAudioStream.getTimebase(), encodeAudioStream.getTimebase());
                encodePacket.setStreamIndex(demuxer.getAudioStreamIndex());
            }
            muxer.writePacket(encodePacket, encodeResult);
        }, &result);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode
    }, &result);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode

    encoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
}

TEST(TRANS_CODE, TRANS_CODE_2) {
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
    videoEncodeParameter.setEncodeThreadCount(10);

    const av::CodecParameters demuxerAudioCodecParameters = demuxer.getAudioCodecParameters();
    const av::Stream& demuxerAudioStream = demuxer.getAudioStream();
    av::AudioEncodeParameters audioEncodeParameter;
    audioEncodeParameter.setBitrate(demuxerAudioCodecParameters.getBitrate());
    audioEncodeParameter.setTimebase(demuxerAudioStream.getTimebase());
    audioEncodeParameter.setSampleFormat(av::SAMPLE_FORMAT::FLTP);
    audioEncodeParameter.setSamplerate(demuxerAudioCodecParameters.getSamplerate());
    audioEncodeParameter.setChannelLayout(demuxerAudioCodecParameters.getChannelLayout());

    av::CodecContextPtr encodeVideoCodecContext = av::createVideoEncodeContext(av::VIDEO_CODEC_ID::H264, videoEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr encodeAudioCodecContext = av::createAudioEncodeContext(av::AUDIO_CODEC_ID::AAC, audioEncodeParameter, &result);
    ASSERT_TRUE(result.isSuccess());

    av::Muxer muxer;
    muxer.open(TEST::TRANSCODING_MP4_FILE_2, &result);
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


    av::Packet packet;
    while (demuxer.read(&packet, &result)) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode
        decoder.decode(packet, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* decodeResult){
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode
            encoder.encode(packet.getMediaType(), decodeFrame, [&](av::Packet &encodePacket, av::AVResult* encodeResult) {
                if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
                    encodePacket.rescaleTS(demuxerVideoStream.getTimebase(), encodeVideoStream.getTimebase());
                    encodePacket.setStreamIndex(demuxer.getVideoStreamIndex());
                } else if (packet.getMediaType() == av::MEDIA_TYPE::AUDIO) {
                    encodePacket.rescaleTS(demuxerAudioStream.getTimebase(), encodeAudioStream.getTimebase());
                    encodePacket.setStreamIndex(demuxer.getAudioStreamIndex());
                }
                muxer.writePacket(encodePacket, encodeResult);
            }, &result);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode
            ASSERT_TRUE(result.isSuccess());
        }, &result);
        ASSERT_TRUE(result.isSuccess());
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode
    }
    ASSERT_TRUE(result.isSuccess());

    decoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
    encoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
}