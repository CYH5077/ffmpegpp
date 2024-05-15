set(SOURCES
        impl/DnnSuperResImpl.cpp
        impl/CVMatImpl.cpp

        CVMat.cpp
        DnnSuperRes
        EDSRSuperess.cpp
        ESPCNSuperess.cpp
        FSRCNNSuperess.cpp
)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
    list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})