set(SOURCES
        FFAVPictureFormat.cpp 

        FFAVRational.cpp
        
        FFAVPacket.cpp
        impl/FFAVPacketImpl.cpp

        FFAVInputContext.cpp
        FFAVOutputContext.cpp
        impl/FFAVFormatContextImpl.cpp

        FFAVCodecParameters.cpp
        impl/FFAVCodecParametersImpl.cpp

        FFAVFrame.cpp
        impl/FFAVFrameImpl.cpp

        FFAVStream.cpp
        impl/FFAVStreamImpl.cpp

        FFAVChannelLayout.cpp
        impl/FFAVChannelLayoutImpl.cpp

        FFAVSwsContext.cpp
        impl/FFAVSwsContextImpl.cpp

        FFAVCodecContext.cpp
        impl/FFAVCodecContextImpl.cpp)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
        list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()
 
target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})