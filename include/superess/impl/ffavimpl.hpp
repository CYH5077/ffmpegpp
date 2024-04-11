#pragma once

#include <memory>

namespace ff::dnn {
    class DnnSuperResImpl;
    class CVMatImpl;

    typedef std::shared_ptr<DnnSuperResImpl> DnnSuperResImplPtr;
    typedef std::shared_ptr<CVMatImpl> CVMatImplPtr;
};