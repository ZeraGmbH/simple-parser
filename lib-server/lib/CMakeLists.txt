set(HEADERS
    cmdparserbase.h
    cmdclient.h
    cmdhandlerbase.h
    cmdserver.h
    QSimpleCmdHandlerBase
    QSimpleCmdParserSocketBase
    QSimpleCmdServer
)

set(SOURCES
    cmdclient.cpp
    cmdhandlerbase.cpp
    cmdparserbase.cpp
    cmdserver.cpp
)

add_library(simplecmdiobase STATIC
    ${HEADERS}
    ${SOURCES}
)

set_target_properties(simplecmdiobase PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simplecmdiobase PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries (simplecmdiobase
    PUBLIC
    ${QTX}::Core
    ${QTX}::Network
    # ask linker to help us finding unresolved symbols
    "-Wl,--no-undefined"
    PRIVATE
    systemd
)

target_include_directories(simplecmdiobase
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/simplecmdiobase>
)

generate_export_header(simplecmdiobase)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/simplecmdiobase_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplecmdiobase
    )

install(TARGETS simplecmdiobase
    EXPORT simplecmdiobase
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplecmdiobase
)
