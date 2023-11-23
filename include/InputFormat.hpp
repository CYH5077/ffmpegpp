#pragma once

#include "AVResult.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {


class InputFormat {
public:
    explicit InputFormat();
    virtual ~InputFormat();

public:
    bool open(const std::string& fileName, AVResult& result);
    void close();

    void printDump();
    
public: // Raw Pointer
    AVFormatContext* getRaw();


private:
    bool createFormatContext(AVResult& result);
    bool openFormatContext(const std::string& fileName, AVResult& result);

private:
    AVFormatContext* formatContext;
};


};