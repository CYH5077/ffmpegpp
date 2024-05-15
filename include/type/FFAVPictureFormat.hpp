#pragma once

namespace ff {
    enum class PICTURE_FORMAT {
        RGB,
        YUV420P,

        ETC
    };

    int PICTURE_FORMAT_TO_AV_FORMAT(PICTURE_FORMAT format);
};