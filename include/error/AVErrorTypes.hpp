#pragma once

namespace ff {
    enum class AV_ERROR_TYPE {
        SUCCESS,
        FAILED,

        AV_EAGAIN,
        AV_EOF,
        AV_ERROR,
    };
}

