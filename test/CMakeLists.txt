set(SOURCES main.cpp
            transmuxing.cpp
            transcode.cpp
            split_video.cpp)

set(FFMPEGPP_TEST ffmpegpp_test)
#enable_testing() # build/test 폴더 생성됨

add_executable(${FFMPEGPP_TEST} ${SOURCES})

find_package(GTEST REQUIRED)
target_link_libraries(${FFMPEGPP_TEST} PRIVATE ${PROJECT_NAME} 
                                            GTEST::gtest 
                                            GTEST::gtest_main 
                                            GTEST::gmock 
                                            GTEST::gmock_main)
target_include_directories(${FFMPEGPP_TEST} PRIVATE ${CMAKE_SOURCE_DIR}/include)


#add_custom_command(TARGET ${FFMPEGPP_TEST} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${FFMPEGPP_TEST}> ${CMAKE_SOURCE_DIR}/${FFMPEGPP_TEST}
#    COMMENT "ffmpegpp_test Copy"
#)

install(TARGETS ${FFMPEGPP_TEST}
        DESTINATION ${INSTALL_DIR}/bin)