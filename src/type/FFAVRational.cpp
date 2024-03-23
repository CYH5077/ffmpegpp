#include "type/FFAVRational.hpp"

namespace ff {
    FFAVRational::FFAVRational() : num(0), den(0) {}
    FFAVRational::FFAVRational(FFAVRational& rational) : num(rational.num), den(rational.den) {}
    FFAVRational::~FFAVRational() {}

    int FFAVRational::getNum() { return num; }
    int FFAVRational::getDen() { return den; }

    void FFAVRational::setNum(int num) { this->num = num; }
    void FFAVRational::setDen(int den) { this->den = den; }
};