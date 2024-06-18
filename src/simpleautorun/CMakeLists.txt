set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

#set target Version
set_target_properties(simpleautorunlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simpleautorunlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries (simpleautorunlib
    PUBLIC
    simplecmdiobase
)

# announce headers - target perspective
target_include_directories(simpleautorunlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/simpleautorunlib>
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    simpleautorunlibConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

# configure *Config.cmake
configure_file(simpleautorunlibConfig.cmake.in simpleautorunlibConfig.cmake @ONLY)

# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/simpleautorunlibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/simpleautorunlibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpleautorunlib
    )

# install targets cmake-files
install(EXPORT simpleautorunlib
    FILE simpleautorunlibTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpleautorunlib
    )

#generate export header
#include(GenerateExportHeader)
#generate_export_header(simpleautorunlib)

#install export header
#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/simpleautorunlib_export.h
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simpleautorunlib
#    )


# install library
install(TARGETS simpleautorunlib
    EXPORT simpleautorunlib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install public headers
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simpleautorunlib
)

add_executable(simpleautorun
    main.cpp
)

target_link_libraries(simpleautorun
    PUBLIC
    simpleautorunlib
)

install(TARGETS simpleautorun
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

