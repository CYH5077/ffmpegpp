#pragma once

#include "AVType.hpp"
#include "Rational.hpp"

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

    public: // getter setter
        Rational getTimeBase();

        void setTimeBase(Rational& timebase);

    public: // Raw pointer
        AVFrame*  getRawFrame();

    private:
        AVFrame* frame;

        // Decoder 에서 저장됨
        Rational timebase;
    };
};