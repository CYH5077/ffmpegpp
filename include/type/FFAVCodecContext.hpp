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

    public:  // get set
        FFAVCodecContextImplPtr getImpl();

        void setCudaFormat(int cudaFormat);
        int getCudaFormat();

        bool isCudaFormat();

        AVError setOpt(const std::string& key, const std::string& value);

    public:
        bool findCudaFormat();
       
    private:
        int cudaFormat;

        FFAVCodecContextImplPtr codecContextImpl;
    };

    using FFAVCodecContextList = std::vector<FFAVCodecContextPtr>;
}
