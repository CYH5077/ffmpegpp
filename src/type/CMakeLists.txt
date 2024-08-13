set(SOURCES
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

        impl/FFAVStreamImpl.cpp
        FFAVDecodeStream.cpp
        FFAVEncodeStream.cpp

        FFAVChannelLayout.cpp
        impl/FFAVChannelLayoutImpl.cpp

        FFAVCodecContext.cpp
        impl/FFAVCodecContextImpl.cpp

        FFAVSwsContext.cpp
        impl/FFAVSwsContextImpl.cpp
)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
        list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()
 
target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})