cmake_minimum_required(VERSION 3.6)

project(ffmpegcpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_CXX_FLAGS_DEBUG -g)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/build) 

add_subdirectory(src)
add_subdirectory(test)