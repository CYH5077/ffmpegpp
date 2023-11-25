#pragma once

#include "AVType.hpp"


namespace av {

class Frame {
public:
    explicit Frame();
    explicit Frame(AVFrame* frame);
    virtual ~Frame();

public:
    Frame(const Frame&) = delete;
    Frame& operator=(const Frame&) = delete;

public:
    void unref();

    void printDump();
    
public: // Raw pointer
    AVFrame* getRawFrame();
    
private:
    AVFrame* frame;
};

};