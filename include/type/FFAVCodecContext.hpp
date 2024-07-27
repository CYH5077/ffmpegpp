#pragma once

#include <memory>
#include <vector>

#include "error/ffav.hpp"
#include "type/FFAVStream.hpp"
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

    public:  // get set
        FFAVCodecContextImplPtr getImpl();
        int getStreamIndex();
        FFAVStreamPtr getDecodeStream();
        FFAVStreamPtr getEncodeStream();

        void setStreamIndex(int streamIndex);
        void setDecodeStream(FFAVStreamPtr stream);
        void setEncodeStream(FFAVStreamPtr stream);

        int getWidth();
        int getHeight();

        AVError setOpt(const std::string& key, const std::string& value);

    public:
        // Cuda
        void setEnableCuda(bool flag);
        bool isEnableCuda();
        int getCudaHWFormat();
        bool findCUDAHWFormat();

    private:
        bool isEnableCudaFlag;
        int cudaHWFormat;

        int streamIndex;

        FFAVStreamPtr decodeStream;
        FFAVStreamPtr encodeStream;

        FFAVCodecContextImplPtr codecContextImpl;
    };

    using FFAVCodecContextList = std::vector<FFAVCodecContextPtr>;
}
