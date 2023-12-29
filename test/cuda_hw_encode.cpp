#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "ffmpegpp.hpp"

TEST(CUDA, CUDA_TRANSCODE) {
    ASSERT_TRUE(av::isCudaVideoEncodingDecodingAvailable());

    av::AVResult result;

    // Demuxer
    av::Demuxer demuxer;
    demuxer.open(TEST::MP4_FILE, &result);
    av::CodecContextPtr decodeVideoCodecContext = av::createVideoCUDADecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());
    av::CodecContextPtr decodeAudioCodecContext = av::createAudioDecodeContext(demuxer, &result);
    ASSERT_TRUE(result.isSuccess());


    av::CodecContextPtr encodeVideoCodecContext = nullptr;
    av::CodecContextPtr encodeAudioCodecContext = nullptr;

    av::VideoStreamInfoPtr demuxerVideoStreamInfo = demuxer.getVideoStreamInfo();
    if (demuxerVideoStreamInfo != nullptr) {
        av::VideoEncodeParameters videoEncodeParameter;
        videoEncodeParameter.setBitrate(demuxerVideoStreamInfo->getBitrate());
        videoEncodeParameter.setWidth(demuxerVideoStreamInfo->getWidth());
        videoEncodeParameter.setHeight(demuxerVideoStreamInfo->getHeight());
        videoEncodeParameter.setTimeBase(demuxerVideoStreamInfo->getTimebase());
        videoEncodeParameter.setFrameRate(demuxerVideoStreamInfo->getFramerate());
        videoEncodeParameter.setGOPSize(10);
        videoEncodeParameter.setMaxBFrames(0);
        videoEncodeParameter.setPixelFormat(av::PIXEL_FORMAT::YUV420P);
        videoEncodeParameter.setEncodeThreadCount(10);

        encodeVideoCodecContext = av::createVideoCUDAEncoderContext(av::VIDEO_HW_CODEC_ID::H264,videoEncodeParameter, &result);
        ASSERT_TRUE(result.isSuccess());
    }

    av::AudioStreamInfoPtr demuxerAudioStreamInfo = demuxer.getAudioStreamInfo();
    if (demuxerAudioStreamInfo != nullptr) {
        av::AudioEncodeParameters audioEncodeParameter;
        audioEncodeParameter.setBitrate(demuxerAudioStreamInfo->getBitrate());
        audioEncodeParameter.setTimebase(demuxerAudioStreamInfo->getTimebase());
        audioEncodeParameter.setSampleFormat(av::SAMPLE_FORMAT::FLTP);
        audioEncodeParameter.setSamplerate(demuxerAudioStreamInfo->getSamplerate());
        audioEncodeParameter.setChannelLayout(demuxerAudioStreamInfo->getChannelLayout());

        encodeAudioCodecContext = av::createAudioEncodeContext(av::AUDIO_CODEC_ID::AAC,audioEncodeParameter, &result);
        ASSERT_TRUE(result.isSuccess());
    }



    av::Muxer muxer;
    muxer.open(TEST::CUDA_TRANSCODING_MP4_FILE_1, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.createNewStream(encodeVideoCodecContext, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.createNewStream(encodeAudioCodecContext, &result);
    ASSERT_TRUE(result.isSuccess());
    muxer.writeHeader(&result);
    ASSERT_TRUE(result.isSuccess());

    const av::Stream &encodeVideoStream = muxer.getVideoStream();
    const av::Stream &encodeAudioStream = muxer.getAudioStream();

    av::Encoder encoder(encodeVideoCodecContext, encodeAudioCodecContext);
    av::Decoder decoder(decodeVideoCodecContext, decodeAudioCodecContext);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode
    decoder.decode(demuxer, [&](av::Packet& packet, av::Frame& decodeFrame, av::AVResult* decodeReuslt) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode
        encoder.encode(packet.getMediaType(), decodeFrame,
                       [&](av::Packet &encodePacket, av::AVResult *encodeResult) {
                           if (packet.getMediaType() == av::MEDIA_TYPE::VIDEO) {
                               encodePacket.rescaleTS(demuxerVideoStreamInfo->getTimebase(),encodeVideoStream.getTimebase());
                               encodePacket.setStreamIndex(demuxer.getVideoStreamIndex());
                           } else if (packet.getMediaType() == av::MEDIA_TYPE::AUDIO) {
                               encodePacket.rescaleTS(demuxerAudioStreamInfo->getTimebase(),encodeAudioStream.getTimebase());
                               encodePacket.setStreamIndex(demuxer.getAudioStreamIndex());
                           }
                           muxer.writePacket(encodePacket, encodeResult);
                       }, decodeReuslt);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Encode
    }, &result);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Decode

    encoder.flush(&result);
    ASSERT_TRUE(result.isSuccess());
}