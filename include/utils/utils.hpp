#pragma once

#include "type/ffav.hpp"

#include <vector>

namespace ff {
    void ffmpegLogOff();

    AVError copyStream(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>* streamMapper = nullptr);
    AVError copyPackets(FFAVInputContext& inputContext, FFAVOutputContext* outputContext, std::vector<int>& streamMapper);
}