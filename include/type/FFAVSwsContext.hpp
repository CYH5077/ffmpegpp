#pragma once

#include <memory>

#include "common/FFAVPictureFormat.hpp"
#include "error/ffav.hpp"
#include "type/FFAVFrame.hpp"
#include "type/impl/ffavimpl.hpp"

namespace ff {

    class FFAVSwsContext;
    typedef std::shared_ptr<FFAVSwsContext> FFAVSwsContextPtr;

    class FFAVSwsContext : public std::enable_shared_from_this<FFAVSwsContext> {
    public:
        explicit FFAVSwsContext(int width, int height, int avPictureFormat);
        explicit FFAVSwsContext(int width, int height, PICTURE_FORMAT format);
        virtual ~FFAVSwsContext();

    public:
        static FFAVSwsContextPtr create(int width, int height, PICTURE_FORMAT format = PICTURE_FORMAT::YUV420P);
        static FFAVSwsContextPtr create(int width, int height, int avPictureFormat);

    public:
        AVError convert(FFAVFrame* frame);

    public:  // get set
        FFAVSwsContextImplPtr getImpl();

        int getWidth();
        int getHeight();

    private:
        AVError newSwsContext(FFAVFrame& frame);

    private:
        FFAVSwsContextImplPtr swsContextImpl;

        int width;
        int height;

        int avPictureFormat;
    };
};
