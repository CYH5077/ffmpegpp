set(SOURCES main.cpp
            transmuxing.cpp
            transcode.cpp
            split_video.cpp
        play_pause_stop.cpp)

set(FFMPEGPP_TEST ffmpegpp_test)
#enable_testing() # build/test 폴더 생성됨

add_executable(${FFMPEGPP_TEST} ${SOURCES})

target_link_libraries(${FFMPEGPP_TEST} PRIVATE ${PROJECT_NAME} 
                                               gtest
                                               gtest_main
                                               gmock
                                               gmock_main)

install(TARGETS ${FFMPEGPP_TEST}
        DESTINATION ${INSTALL_DIR}/test)