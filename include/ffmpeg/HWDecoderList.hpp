#pragma once

#include <vector>
#include <string>

namespace av {

    class HWDecoderList : public std::vector<std::string> {
    public:
        explicit HWDecoderList();
        virtual ~HWDecoderList();
    };

};