#include "type/FFAVCodecContext.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"


namespace ff {
    FFAVCodecContextPtr FFAVCodecContext::create() {
        return std::make_shared<FFAVCodecContext>();
    }


    FFAVCodecContext::FFAVCodecContext() {
        this->codecContextImpl = FFAVCodecContextImpl::create();
    }

    FFAVCodecContext::~FFAVCodecContext() {

    }

    FFAVCodecContextImplPtr FFAVCodecContext::getImpl() {
        return this->codecContextImpl;
    }
};