#pragma once

#include <memory>
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
        void setErrorCallback(std::function<void(ERROR_TYPE, AVError&)> errorCallback);

        void transcode(const std::string& outputFile);
        void stop();

        void callErrorCallback(ERROR_TYPE type, AVError& error);

    private:
        void wait();
        void closeNotify();

    private:  // Thread
        AVError runDecode();
        AVError runEncode(const std::string& outputFile);

    private:
        FFAVTranscoderParameter& parameter;

        std::shared_ptr<FFAVDecoder> decoder;
        std::shared_ptr<FFAVEncoder> encoder;

        std::function<void(ERROR_TYPE, AVError&)> errorCallback;

        bool isDecoderThreadRunning;
        bool isTranscodeRunning;
        std::vector<std::shared_ptr<std::thread>> threads;
        FFAVThreadSafeQueue<FFAVFrame> encodeQueue;
    };
};
