#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "codecs/ffav.hpp"
#include "error/ffav.hpp"
#include "high_transcode/FFAVThreadSafeQueue.hpp"
#include "high_transcode/FFAVTranscoderParameter.hpp"
#include "type/ffav.hpp"

namespace ff {
    enum class ERROR_TYPE {
        DECODE_THREAD,
        ENCODE_THREAD,
    };

    class FFAVTranscoder {
    public:
        explicit FFAVTranscoder(FFAVTranscoderParameter& parameter);
        virtual ~FFAVTranscoder();

    public:
        void transcode(const std::string& outputFile, OUTPUT_TYPE type = OUTPUT_TYPE::FILE);
        void stop();

    public:
        void setErrorCallback(std::function<void(ERROR_TYPE, AVError&)> errorCallback);
        void setSuccessCallback(std::function<void()> successCallback);
        void setDecodeCallback(std::function<void(FFAVFrame&)> decodeCallback);
        void setEncodeCallback(std::function<void(FFAVPacket&)> encodeCallback);
        void setFinishCallback(std::function<void()> finishCallback);

        void setOutputContextOpt(const std::string& key, const std::string& value);
        void setInputContextOpt(const std::string& key, const std::string& value);
        void setVideoEncodeContextOpt(const std::string& key, const std::string& value);
        void setAudioEncodeContextOpt(const std::string& key, const std::string& value);

        void callErrorCallback(ERROR_TYPE type, AVError& error);

    private:
        void wait();
        void closeNotify();

    private:  // Thread
        AVError runDecode();
        AVError runEncode(const std::string& outputFile, OUTPUT_TYPE type);

    private:
        AVError setOptions(FFAVOutputContext& outputContext, OUTPUT_TYPE type);

    private:
        FFAVTranscoderParameter& parameter;

        std::shared_ptr<FFAVDecoder> decoder;
        std::shared_ptr<FFAVEncoder> encoder;
        ff::FFAVSwsContext swsContext;

        std::function<void()> successCallback;
        std::function<void(ERROR_TYPE, AVError&)> errorCallback;
        std::function<void(FFAVFrame&)> decodeCallback;
        std::function<void(FFAVPacket&)> encodeCallback;
        std::function<void()> finishCallback;

        // Transcode Thread
        bool isDecoderThreadRunning;
        bool isTranscodeRunning;
        std::vector<std::shared_ptr<std::thread>> threads;
        FFAVThreadSafeQueue<FFAVFrame> encodeQueue;

        // Options;
        std::unordered_map<std::string, std::string> outputContextOpt;
        std::unordered_map<std::string, std::string> inputContextOpt;
        std::unordered_map<std::string, std::string> videoEncodeContextOpt;
        std::unordered_map<std::string, std::string> audioEncodeContextOpt;
    };
};
