#pragma once

#include <memory>

#include "common/FFAVType.hpp"

namespace ff {
    class FFAVPacketImpl;
    class FFAVFormatContextImpl;
    class FFAVCodecContextImpl;
    class FFAVFrameImpl;
    class FFAVStreamImpl;
    class FFAVChannelLayoutImpl;
    class FFAVCodecParametersImpl;
    class FFAVSwsContextImpl;
    class FFAVFilterContextImpl;

    using FFAVPacketImplPtr = std::shared_ptr<FFAVPacketImpl>;
    using FFAVFormatContextImplPtr = std::shared_ptr<FFAVFormatContextImpl>;
    using FFAVCodecContextImplPtr = std::shared_ptr<FFAVCodecContextImpl>;
    using FFAVFrameImplPtr = std::shared_ptr<FFAVFrameImpl>;
    using FFAVStreamImplPtr = std::shared_ptr<FFAVStreamImpl>;
    using FFAVChannelLayoutImplPtr = std::shared_ptr<FFAVChannelLayoutImpl>;
    using FFAVCodecParametersImplPtr = std::shared_ptr<FFAVCodecParametersImpl>;
    using FFAVSwsContextImplPtr = std::shared_ptr<FFAVSwsContextImpl>;
    using FFAVFilterContextImplPtr = std::shared_ptr<FFAVFilterContextImpl>;
};
