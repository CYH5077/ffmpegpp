#pragma once

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

class Rational {
public:
    explicit Rational();
    explicit Rational(int num, int den);
    virtual ~Rational();

public: // getter setter
    int getNum() const;
    int getDen() const;

    void setNum(int num);
    void setDen(int den);
    
private:
    AVRational rational;
};

};