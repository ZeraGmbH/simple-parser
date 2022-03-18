set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    cmdparserbase.h
    cmdclient.h
    cmdhandlerbase.h
    cmdserver.h
)

set(SOURCES
    cmdclient.cpp
    cmdhandlerbase.cpp
    cmdparserbase.cpp
    cmdserver.cpp
)

add_library(simplecmdiobase SHARED
    ${HEADERS}
    ${SOURCES}
)

#set target Version
set_target_properties(simplecmdiobase PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simplecmdiobase PROPERTIES SOVERSION ${SIMPLEPARSER_VERSION_MAJOR})

target_link_libraries (simplecmdiobase
    PUBLIC
    Qt5::Core
    Qt5::Network
    # ask linker to help us finding unresolved symbols
    "-Wl,--no-undefined"
)

# announce headers - target perspective
target_include_directories(simplecmdiobase
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SimpleCmdioBase>
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS simplecmdiobase
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# install library
install(TARGETS simplecmdiobase
    EXPORT SimpleCmdioBase
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SimpleCmdioBase
)
