#pragma once

#include "type/ffav.hpp"
#include "error/ffav.hpp"

#include <string>

namespace ff {
    AVError transMux(FFAVInputContext& inputContext, std::string&  outputFilename);
    AVError transMux(FFAVInputContext& inputContext, std::string&& outputFilename);
    AVError transMux(std::string& inputFilename , std::string& outputFilename);
};