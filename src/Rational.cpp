#include "Rational.hpp"

namespace av {

Rational::Rational() {

}

Rational::Rational(int num, int den) {
    this->rational.num = num;
    this->rational.den = den;
}

Rational::~Rational() {

}

int Rational::getNum() const {
    return this->rational.num;
}

int Rational::getDen() const {
    return this->rational.den;
}

void Rational::setNum(int num) {
    this->rational.num = num;
}

void Rational::setDen(int den) {
    this->rational.den = den;
}

};