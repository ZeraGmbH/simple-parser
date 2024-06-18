set(HEADERS
    cmdparserfile.h
    cmdhandlerfile.h
    commonhelpers.h
)

set(SOURCES
    cmdparserfile.cpp
    cmdhandlerfile.cpp
    commonhelpers.cpp
)

add_library(simpleautorunlib SHARED
    ${HEADERS}
    ${SOURCES}
)

set_target_properties(simpleautorunlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simpleautorunlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries (simpleautorunlib
    PUBLIC
    simplecmdiobase
)
