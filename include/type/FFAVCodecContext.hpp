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

        AVError setOpt(const std::string& key, const std::string& value);

    private:
        FFAVCodecContextImplPtr codecContextImpl;
    };

    using FFAVCodecContextList = std::vector<FFAVCodecContextPtr>;
}
