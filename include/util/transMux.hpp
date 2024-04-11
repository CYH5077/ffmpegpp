#pragma once

#include "type/ffav.hpp"
#include "error/ffav.hpp"

#include <string>

namespace ff {
    AVError transMux(FFAVInputContext& inputContext, const std::string&  outputFilename);
    AVError transMux(const std::string& inputFilename , const std::string& outputFilename);
};