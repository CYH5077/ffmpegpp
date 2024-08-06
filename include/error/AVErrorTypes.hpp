#pragma once

namespace ff {
    enum class AV_ERROR_TYPE {
        SUCCESS,
        FAILED,
        INVALID_ARGUMENT,
        USER_STOP,

        AV_DECODE_FINISH,
        AV_EAGAIN,
        AV_EOF,
        AV_ERROR,
    };
}

