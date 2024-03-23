set(SOURCES
        FFAVRational.cpp

        FFAVPacket.cpp
        impl/FFAVPacketImpl.cpp
 
        FFAVFormatContext.cpp
        impl/FFAVFormatContextImpl.cpp

        FFAVFrame.cpp
        impl/FFAVFrameImpl.cpp

        FFAVStream.cpp
        impl/FFAVStreamImpl.cpp

        FFAVChannelLayout.cpp
        impl/FFAVChannelLayoutImpl.cpp

        FFAVCodecContext.cpp
        impl/FFAVCodecContextImpl.cpp)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
        list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})