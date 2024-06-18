file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(simplecmdio-testlib STATIC
    ${SOURCES}
    ${HEADER}
)

target_include_directories(simplecmdio-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(simplecmdio-testlib
    PUBLIC
    simplecmdiobase
)
