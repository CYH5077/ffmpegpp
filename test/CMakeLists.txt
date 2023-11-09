set(SOURCES main.cpp
            TEST.cpp)

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

add_test(AllTestsInMain ${FFMPEGPP_TEST})
