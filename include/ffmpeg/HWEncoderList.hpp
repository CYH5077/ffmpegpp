#pragma once

#include <vector>
#include <string>

namespace av {
    class HWEncoderList : public std::vector<std::string> {
    public:
        explicit HWEncoderList();
        virtual ~HWEncoderList();
    };
};