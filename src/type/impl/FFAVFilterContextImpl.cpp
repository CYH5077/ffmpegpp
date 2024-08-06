#include "type/impl/FFAVFilterContextImpl.hpp"

extern "C" {
#include "libavfilter/avfilter.h"
}

static AVFilterContext* newAVFilterContext() {}

static void freeAVFilterContext(AVFilterContext* filterContext) {}

namespace ff {
    FFAVFilterContextImplPtr FFAVFilterContextImpl::create() {
        return std::make_shared<FFAVFilterContextImpl>();
    }

    FFAVFilterContextImpl::FFAVFilterContextImpl() {
        filterContext = AVFilterContextPtr(newAVFilterContext(), freeAVFilterContext);
    }

    FFAVFilterContextImpl::~FFAVFilterContextImpl() {}

    AVFilterContextPtr FFAVFilterContextImpl::getRaw() {
        return this->filterContext;
    }
}
