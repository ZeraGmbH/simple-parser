file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(simplecmdio-client SHARED
    ${SOURCES}
    ${HEADER}
    )

target_include_directories(simplecmdio-client
    PUBLIC
    $<INSTALL_INTERFACE:include/simplecmdio-client>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(simplecmdio-client
    PUBLIC
    Qt5::Core
    Qt5::Network
    Zera::zera-tasks-base
    )

set_target_properties(simplecmdio-client PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simplecmdio-client PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS simplecmdio-client
    EXPORT simplecmdiobase
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplecmdio-client
    )
