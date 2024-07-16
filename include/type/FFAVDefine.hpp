#pragma once

namespace ff {
    enum class DATA_TYPE { VIDEO, AUDIO, SUBTITLE, DATA, UNKNOWN };

    DATA_TYPE DATA_TYPE_FROM_AV_CODEC_TYPE(int codecType);
}
