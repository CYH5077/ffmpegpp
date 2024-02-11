set(SOURCES main.cpp
            transmuxing.cpp
            transcode.cpp
            play_pause_stop.cpp
            decode.cpp
            cuda_support.cpp
            cuda_hw_encode.cpp
            cuda_hw_decode.cpp)

set(FFMPEGPP_TEST ffmpegpp_test)
#enable_testing() # build/test 폴더 생성됨

add_executable(${FFMPEGPP_TEST} ${SOURCES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${FFMPEGPP_TEST} PRIVATE  ${PROJECT_NAME}
                                                GTest::gtest
                                                GTest::gtest_main
                                                GTest::gmock
                                                GTest::gmock_main)
target_include_directories(${FFMPEGPP_TEST} PRIVATE ${FFMPEGPP_INCLUDE_DIR})

install(TARGETS ${FFMPEGPP_TEST}
        DESTINATION ${FFMPEGPP_INSTALL_DIR}/test)