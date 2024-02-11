cmake_minimum_required(VERSION 3.6)

project(ffmpegpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set (CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")
    set (CMAKE_CXX_FLAGS_DEBUG "/MD /Zi /Ob0 /Od /RTC1")
elseif (LINUX)
    set (CMAKE_CXX_FLAGS -Wall)
	set (CMAKE_CXX_FLAGS_RELEASE "-O3")
	set (CMAKE_CXX_FLAGS_DEBUG "-g")
endif ()

set(FFMPEGPP_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/ffmpegpp)
set(FFMPEGPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_library(${PROJECT_NAME})

find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${FFMPEGPP_INCLUDE_DIR}
										           ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${FFMPEG_LIBRARY_DIRS}) 
target_link_libraries(${PROJECT_NAME} PUBLIC ${FFMPEG_LIBRARIES})


add_subdirectory (src)
add_subdirectory (test)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${FFMPEGPP_INSTALL_DIR}/lib)
install(DIRECTORY ${FFMPEGPP_INCLUDE_DIR}
        DESTINATION ${FFMPEGPP_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.hpp")
