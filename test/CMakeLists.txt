set(SOURCES main.cpp
            video_format.cpp)

set(FFMPEGPP_TEST ffmpegpp_test)
enable_testing()

add_executable(${FFMPEGPP_TEST} ${SOURCES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${FFMPEGPP_TEST} PRIVATE ${PROJECT_NAME} 
                                            GTest::gtest 
                                            GTest::gtest_main 
                                            GTest::gmock 
                                            GTest::gmock_main)
target_include_directories(${FFMPEGPP_TEST} PRIVATE ${CMAKE_SOURCE_DIR}/include)


add_custom_command(TARGET ${FFMPEGPP_TEST} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${FFMPEGPP_TEST}> ${CMAKE_SOURCE_DIR}/${FFMPEGPP_TEST}
    COMMENT "ffmpegpp_test Copy"
)