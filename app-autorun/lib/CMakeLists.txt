file(GLOB SOURCES
    *.cpp
)

file(GLOB HEADER
    *.h
)

add_library(simpleautorunlib STATIC
    ${HEADERS}
    ${SOURCES}
)

set_target_properties(simpleautorunlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simpleautorunlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries (simpleautorunlib
    PUBLIC
    simplecmdiobase
)

target_include_directories(simpleautorunlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
