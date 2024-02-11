#pragma once

#include "ffmpeg/AVType.hpp"
#include "ffmpeg/Rational.hpp"

namespace av {
    class Frame {
    public:
        explicit Frame();
        explicit Frame(const Frame& frame);
        explicit Frame(AVFrame* frame);
        virtual ~Frame();

    public:
        Frame& operator=(const Frame&) = delete;

    public:
        void unref();

    public: // getter setter
        const Rational& getTimeBase();
        unsigned char*  getData(int index);
        int getWidth();
        int getHeight();
        int getLineSize(int index);
        long long getPTS();
        

        void setTimeBase(Rational& timebase);


    public: // Raw pointer
        AVFrame*  getRawFrame();
        int       getRawPixelFormat();

    private:
        AVFrame* frame;

        // Decoder 에서 저장됨
        Rational timebase;
    };
};