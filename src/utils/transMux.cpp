#include "utils/transMux.hpp"

#include "utils/utils.hpp"

#include <iostream>
#include <vector>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    AVError transMux(FFAVInputContext& inputContext, const std::string& outputFilename) {
        std::vector<int> streamMapper;
        FFAVOutputContext outputContext;

        AVError error = outputContext.open(outputFilename);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        error = copyStream(inputContext, &outputContext, &streamMapper);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        error = outputContext.writeHeader();
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        error = copyPackets(inputContext, &outputContext, streamMapper);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError transMux(FFAVInputContext& inputContext, const std::string&& outputFilename) {
        return transMux(inputContext, outputFilename);
    }

    AVError transMux(const std::string& inputFilename , const std::string& outputFilename) {
        FFAVInputContext inputContext;
        AVError error = inputContext.open(inputFilename);
        if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
            return error;
        }

        return transMux(inputContext, outputFilename);
    }


};
