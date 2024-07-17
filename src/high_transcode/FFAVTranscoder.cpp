#include "high_transcode/FFAVTranscoder.hpp"

#include <future>
#include <iostream>
#include <memory>

namespace ff {
    FFAVTranscoder::FFAVTranscoder(FFAVTranscoderParameter& parameter)
        : parameter(parameter),
          encodeQueue(200),
          swsContext(parameter.getVideoEncodeParameters()->getWidth(),
                     parameter.getVideoEncodeParameters()->getHeight(),
                     ff::PICTURE_FORMAT::YUV420P) {
        this->decoder =
            std::make_shared<FFAVDecoder>(parameter.getVideoDecodeContext(), parameter.getAudioDecodeContext());
        this->encoder =
            std::make_shared<FFAVEncoder>(parameter.getVideoEncodeContext(), parameter.getAudioEncodeContext());

        this->isTranscodeRunning = false;
        this->isDecoderThreadRunning = false;
    }

    FFAVTranscoder::~FFAVTranscoder() {}

    void FFAVTranscoder::setErrorCallback(std::function<void(ERROR_TYPE, AVError&)> errorCallback) {
        this->errorCallback = errorCallback;
    }

    void FFAVTranscoder::setSuccessCallback(std::function<void()> successCallback) {
        this->successCallback = successCallback;
    }

    void FFAVTranscoder::setDecodeCallback(std::function<void(FFAVFrame&)> decodeCallback) {
        this->decodeCallback = decodeCallback;
    }

    void FFAVTranscoder::setEncodeCallback(std::function<void(FFAVPacket&)> encodeCallback) {
        this->encodeCallback = encodeCallback;
    }

    void FFAVTranscoder::setOutputContextOpt(const std::string& key, const std::string& value) {
        this->outputContextOpt[key] = value;
    }

    void FFAVTranscoder::setInputContextOpt(const std::string& key, const std::string& value) {
        this->inputContextOpt[key] = value;
    }

    void FFAVTranscoder::setVideoEncodeContextOpt(const std::string& key, const std::string& value) {
        this->videoEncodeContextOpt[key] = value;
    }

    void FFAVTranscoder::setAudioEncodeContextOpt(const std::string& key, const std::string& value) {
        this->audioEncodeContextOpt[key] = value;
    }

    void FFAVTranscoder::transcode(const std::string& outputFile, OUTPUT_TYPE type) {
        this->isTranscodeRunning = true;

        // Decode Thread
        this->isDecoderThreadRunning = true;
        this->threads.push_back(std::make_shared<std::thread>([this]() {
            AVError error = this->runDecode();
            if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                this->isTranscodeRunning = false;
                this->callErrorCallback(ERROR_TYPE::DECODE_THREAD, error);
            }
        }));

        // Encode Thread
        this->threads.push_back(std::make_shared<std::thread>([this, &outputFile, type]() {
            AVError error = this->runEncode(outputFile, type);
            if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                this->isTranscodeRunning = false;
                this->callErrorCallback(ERROR_TYPE::ENCODE_THREAD, error);
            }
        }));

        this->wait();
    }

    void FFAVTranscoder::stop() {
        this->isTranscodeRunning = false;

        this->closeNotify();

        this->wait();
    }

    void FFAVTranscoder::wait() {
        for (auto& thread : this->threads) {
            if (thread->joinable()) {
                thread->join();
            }
        }
    }

    void FFAVTranscoder::callErrorCallback(ERROR_TYPE type, AVError& error) {
        this->closeNotify();

        if (this->errorCallback) {
            this->errorCallback(type, error);
        }
    }

    void FFAVTranscoder::closeNotify() {
        this->encodeQueue.closeNotify();
    }

    AVError FFAVTranscoder::runDecode() {
        int decodeCount = 0;
        FFAVInputContext& inputContext = this->parameter.getInputContext();
        AVError error = this->decoder->decode(inputContext, [&](FFAVFrame& frame) {
            if (frame.getType() == DATA_TYPE::VIDEO || frame.getType() == DATA_TYPE::AUDIO) {
                this->encodeQueue.push(frame);
            }

            if (this->isTranscodeRunning == false) {
                return AVError(AV_ERROR_TYPE::USER_STOP);
            }

            if (this->decodeCallback) {
                this->decodeCallback(frame);
            }

            return AVError(AV_ERROR_TYPE::SUCCESS);
        });

        this->isDecoderThreadRunning = false;

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVTranscoder::runEncode(const std::string& outputFile, OUTPUT_TYPE type) {
        AVError error;

        FFAVOutputContext outputContext;
        error = outputContext.open(outputFile, type);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        outputContext.createStream(DATA_TYPE::VIDEO, this->parameter.getVideoEncodeContext());
        outputContext.createStream(DATA_TYPE::AUDIO, this->parameter.getAudioEncodeContext());
        this->setOptions(outputContext, type);
        error = outputContext.writeHeader();
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        // Loop
        int encodeCount = 0;
        while (this->isDecoderThreadRunning == true || this->encodeQueue.size() > 0) {
            this->encodeQueue.wait();
            if (this->encodeQueue.isClosed()) {
                return AVError(AV_ERROR_TYPE::SUCCESS);
            }
            auto frame = this->encodeQueue.pop();

            auto videoEncodeParameters = this->parameter.getVideoEncodeParameters();
            if (frame.getType() == DATA_TYPE::VIDEO) {
                if (frame.getWidth() != videoEncodeParameters->getWidth() ||
                    frame.getHeight() != videoEncodeParameters->getHeight()) {
                    this->swsContext.convert(frame);
                }
            }

            //// Encode
            error = this->encoder->encode(frame, [&](FFAVPacket& packet) {
                FFAVInputContext& inputContext = this->parameter.getInputContext();
                if (packet.getType() == DATA_TYPE::VIDEO) {
                    packet.rescaleTS(inputContext.getVideoStream(), outputContext.getVideoStream());
                    packet.setStreamIndex(inputContext.getVideoStreamIndex());
                } else if (packet.getType() == DATA_TYPE::AUDIO) {
                    packet.rescaleTS(inputContext.getAudioStream(), outputContext.getAudioStream());
                    packet.setStreamIndex(inputContext.getAudioStreamIndex());
                }

                error = outputContext.writePacket(packet);
                if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                    return error;
                }

                if (this->encodeCallback) {
                    this->encodeCallback(packet);
                }

                return AVError(AV_ERROR_TYPE::SUCCESS);
            });

            if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                return error;
            }
        }

        this->encoder->flush();

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVTranscoder::setOptions(FFAVOutputContext& outputContext, OUTPUT_TYPE type) {
        for (auto& opt : this->outputContextOpt) {
            outputContext.setOpt(opt.first, opt.second);
        }

        for (auto& opt : this->inputContextOpt) {
            this->parameter.getInputContext().setOpt(opt.first, opt.second);
        }

        for (auto& opt : this->videoEncodeContextOpt) {
            this->parameter.getVideoEncodeContext()->setOpt(opt.first, opt.second);
        }

        for (auto& opt : this->audioEncodeContextOpt) {
            this->parameter.getAudioEncodeContext()->setOpt(opt.first, opt.second);
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}
