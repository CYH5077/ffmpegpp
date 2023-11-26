#pragma once

#include "AVType.hpp"

namespace av {

class Rational {
public:
    explicit Rational();
    explicit Rational(AVRational rational);
    explicit Rational(int num, int den);
    virtual ~Rational();

public: // getter setter
    int getNum() const;
    int getDen() const;

    void setNum(int num);
    void setDen(int den);
    
private:
    int num;
    int den;
};

};