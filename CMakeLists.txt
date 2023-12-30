cmake_minimum_required(VERSION 3.6)

project(ffmpegcpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_CXX_FLAGS_DEBUG -g)

# Argument
set(FFMPEG_INCLUDE_DIR "" CACHE PATH "Path to FFMPEG include directory")
set(FFMPEG_LIBRARY_DIR "" CACHE PATH "Path to FFMPEG library directory")

set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/build)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/googletest/include)
if (WIN32)
	file(GLOB FFMPEG_LIBRARYS "${FFMPEG_LIBRARY_DIR}/*.a")
else()
	file(GLOB FFMPEG_LIBRARYS "${FFMPEG_LIBRARY_DIR}/*.so")
endif()
include_directories(${INCLUDE_DIR}
                    ${GTEST_INCLUDE_DIR}
                    ${FFMPEG_INCLUDE_DIR})
link_libraries(${FFMPEG_LIBRARYS})
add_library(${PROJECT_NAME})

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory (thirdparty/gtest)
add_subdirectory (src)
add_subdirectory (test)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${INSTALL_DIR}/lib)
install(DIRECTORY ${INCLUDE_DIR}
        DESTINATION ${INSTALL_DIR}
        FILES_MATCHING PATTERN "*.hpp")
