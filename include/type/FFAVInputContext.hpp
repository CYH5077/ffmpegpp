#pragma once

#include <memory>
#include <string>
#include <vector>

#include "error/AVError.hpp"
#include "type/FFAVChannelLayout.hpp"
#include "type/FFAVCodecParameters.hpp"
#include "type/FFAVPacket.hpp"
#include "type/FFAVStream.hpp"

namespace ff {
    class FFAVInputContext {
    public:
        explicit FFAVInputContext();
        virtual ~FFAVInputContext();

    public:  // get set
        FFAVFormatContextImplPtr getImpl();

    private:
        FFAVFormatContextImplPtr formatContextImpl;
    };

};
