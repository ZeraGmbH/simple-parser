set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SimpleCmdioBaseConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

# configure *Config.cmake
configure_file(SimpleCmdioBaseConfig.cmake.in SimpleCmdioBaseConfig.cmake @ONLY)

# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SimpleCmdioBaseConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SimpleCmdioBaseConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SimpleCmdioBase
    )

# install targets cmake-files
install(EXPORT SimpleCmdioBase
    FILE SimpleCmdioBaseTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SimpleCmdioBase
    )

#generate export header
include(GenerateExportHeader)
generate_export_header(simplecmdiobase)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/simplecmdiobase_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SimpleCmdioBase
    )


# install library
install(TARGETS simplecmdiobase
    EXPORT SimpleCmdioBase
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install public headers
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SimpleCmdioBase
)
