#pragma once

#include <memory>
#include <vector>

#include "error/AVError.hpp"
#include "type/ffav.hpp"

namespace ff {
    class FFAVOutputContext {
    public:
        explicit FFAVOutputContext();
        virtual ~FFAVOutputContext();

    public: // get set
        FFAVFormatContextImplPtr getImpl();

        AVError setOpt(const std::string& key, const std::string& value);

    private:
        FFAVFormatContextImplPtr formatContextImpl;
    };
};
