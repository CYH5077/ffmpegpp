set (SOURCES AVResult.cpp
             Demuxer.cpp
             Packet.cpp
             CodecContext.cpp
             Decoder.cpp
             Frame.cpp
             Muxer.cpp
             VideoEncodeParameters.cpp
             Rational.cpp
             AVType.cpp
             Encoder.cpp
             Stream.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include 
                                                   ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${INSTALL_DIR}/lib)
install(DIRECTORY ${HEADER_DIR} 
        DESTINATION ${INSTALL_DIR} 
        FILES_MATCHING PATTERN "*.hpp")
        
if (WIN32)
        set(FFMPEG_DLL_DIR ${CMAKE_SOURCE_DIR}/thirdparty/windows/ffmpeg/bin)
        install(DIRECTORY ${FFMPEG_DLL_DIR} 
                DESTINATION ${INSTALL_DIR}
                FILES_MATCHING PATTERN "*.dll")
endif()