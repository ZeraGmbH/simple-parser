file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(simplecmdio-testlib SHARED
    ${SOURCES}
    ${HEADER}
)

set_target_properties(simplecmdio-testlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simplecmdio-testlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(simplecmdio-testlib
    PUBLIC
    simplecmdiobase
)

target_include_directories(simplecmdio-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/simplecmdio-testlib>
)

install(TARGETS simplecmdio-testlib
    EXPORT simplecmdiobase
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplecmdio-testlib
)
