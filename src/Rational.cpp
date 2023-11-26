#include "Rational.hpp"

extern "C" {
    #include "libavformat/avformat.h"
}

namespace av {

Rational::Rational() {

}

Rational::Rational(AVRational rational) {
    this->num = rational.num;
    this->den = rational.den;
}

Rational::Rational(int num, int den) {
    this->num = num;
    this->den = den;
}

Rational::~Rational() {

}

int Rational::getNum() const {
    return this->num;
}

int Rational::getDen() const {
    return this->den;
}

void Rational::setNum(int num) {
    this->num = num;
}

void Rational::setDen(int den) {
    this->den = den;
}

};