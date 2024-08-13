#pragma once

#include <memory>
#include <vector>

#include "error/ffav.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {
    class FFAVCodecContext;
    typedef std::shared_ptr<FFAVCodecContext> FFAVCodecContextPtr;

    class FFAVCodecContext {
    public:
        explicit FFAVCodecContext();
        virtual ~FFAVCodecContext();

    public:
        static FFAVCodecContextPtr create();

    public:
        bool findCudaFormat();

        AVError openCodec();

    public:  // get set
        FFAVCodecContextImplPtr getImpl();

        int getCudaFormat();
        void setCudaFormat(int cudaFormat);

        std::string getCodecName();
        void setCodecName(const std::string& codecName);

        bool isCodecOpen();
        bool isCudaFormat();

        AVError setOpt(const std::string& key, const std::string& value);

    private:
        int cudaFormat;

        bool isCodecOpenFlag;
        std::string codecName;

        FFAVCodecContextImplPtr codecContextImpl;
    };

    using FFAVCodecContextList = std::vector<FFAVCodecContextPtr>;
}
