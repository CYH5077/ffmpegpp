#include "high_transcode/FFAVTranscoder.hpp"

#include <future>
#include <memory>
#include <iostream>

namespace ff {
    FFAVTranscoder::FFAVTranscoder(FFAVTranscoderParameter& parameter) : parameter(parameter), encodeQueue(150) {
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

    void FFAVTranscoder::transcode(const std::string& outputFile) {
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
        this->threads.push_back(std::make_shared<std::thread>([this, &outputFile]() {
            AVError error = this->runEncode(outputFile);
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

            return AVError(AV_ERROR_TYPE::SUCCESS);
        });

        this->isDecoderThreadRunning = false;

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVTranscoder::runEncode(const std::string& outputFile) {
        AVError error;

        FFAVOutputContext outputContext;
        error = outputContext.open(outputFile);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        outputContext.createStream(DATA_TYPE::VIDEO, this->parameter.getVideoEncodeContext());
        outputContext.createStream(DATA_TYPE::AUDIO, this->parameter.getAudioEncodeContext());
        error = outputContext.writeHeader();
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        int encodeCount = 0;
        FFAVInputContext& inputContext = this->parameter.getInputContext();
        while (this->isDecoderThreadRunning == true ||
               this->encodeQueue.size() > 0) {
            this->encodeQueue.wait();
            if (this->encodeQueue.isClosed()) {
                return AVError(AV_ERROR_TYPE::SUCCESS);
            }
            auto frame = this->encodeQueue.pop();

            error = this->encoder->encode(frame, [&](FFAVPacket& packet) {
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

                return AVError(AV_ERROR_TYPE::SUCCESS);
            });

            if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
				return error;
			}
        }

        this->encoder->flush();

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
}
