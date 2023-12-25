set(SOURCES AVResult.cpp
             Demuxer.cpp
             Packet.cpp
             CodecContext.cpp
             Decoder.cpp
             Frame.cpp
             Muxer.cpp
             VideoEncodeParameters.cpp
             AudioEncodeParameters.cpp
             Rational.cpp
             AVType.cpp
             Encoder.cpp
             Stream.cpp
             CodecParameters.cpp
             ChannelLayout.cpp
)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
        list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})