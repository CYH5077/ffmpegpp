#pragma once

#include "type/impl/ffavimpl.hpp"

extern "C" {
#include "libavfilter/avfilter.h"
}

namespace ff {
    using AVFilterContextPtr = std::shared_ptr<AVFilterContext>;

    class FFAVFilterContextImpl : public std::enable_shared_from_this<FFAVFilterContextImpl> {
    public:
        explicit FFAVFilterContextImpl();
        virtual ~FFAVFilterContextImpl();

    public:
        static FFAVFilterContextImplPtr create();

    public:  // get set
        AVFilterContextPtr getRaw();

    public:
        AVFilterContextPtr filterContext;
    };
};
