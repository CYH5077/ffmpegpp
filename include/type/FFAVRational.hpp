#pragma once

#include "type/ffav.hpp"

namespace ff {
    class FFAVRational {
    public:
        explicit FFAVRational();
        explicit FFAVRational(FFAVRational& rational);

        virtual ~FFAVRational();

    public:
        int getNum();
        int getDen();

        void setNum(int num);
        void setDen(int den);

    private:
        int num;
        int den;
    };
};