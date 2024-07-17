#pragma once

#include <memory>

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

    public:  // get set
        FFAVCodecContextImplPtr getImpl();

        // Cuda
        void setEnableCuda(bool flag);
        bool isEnableCuda();
        int getCudaHWFormat();
        bool findCUDAHWFormat();

        int getWidth();
        int getHeight();

        AVError setOpt(const std::string& key, const std::string& value);

    private:
        bool isEnableCudaFlag;
        int cudaHWFormat;

        FFAVCodecContextImplPtr codecContextImpl;
    };

}
