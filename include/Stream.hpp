#pragma once

#include "AVType.hpp"
#include "Rational.hpp"

namespace av {

class Stream {
public:
    explicit Stream();
    explicit Stream(AVStream* stream);
    virtual ~Stream();

public: // getter setter
    bool isValidStream() const;
    
    Rational getTimebase() const;
    Rational getFramerate() const;

public: // Raw pointer
    AVStream* getRawStream();
    AVCodecParameters* getRawCodecParameters();
    
    void setRawStream(AVStream* stream);

private:
    AVStream* stream;
};


};