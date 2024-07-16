set(SOURCES
        FFAVTranscoder.cpp
        FFAVTranscoderParameter.cpp
)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
    list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})