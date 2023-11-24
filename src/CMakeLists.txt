set (SOURCES AVResult.cpp
             Demuxer.cpp
             Packet.cpp
             CodecContext.cpp
             Decoder.cpp
             Frame.cpp
             Muxer.cpp
             EncodeParameter.cpp
             Rational.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

find_package(x264 CONFIG REQUIRED)
find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include 
                                                   ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})


set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/build)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${INSTALL_DIR}/lib)
install(DIRECTORY ${HEADER_DIR} 
        DESTINATION ${INSTALL_DIR} 
        FILES_MATCHING PATTERN "*.hpp")