if (FFMPEGPP_SUPPRESS)
    add_subdirectory(superess)
endif()

add_subdirectory(codecs)
add_subdirectory(type)
add_subdirectory(error)
add_subdirectory(utils)
add_subdirectory(high_transcode)