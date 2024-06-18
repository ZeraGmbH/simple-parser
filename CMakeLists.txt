cmake_minimum_required(VERSION 3.1)

project(simple-parser LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "3")
set(PROJECT_VERSION_PATCH "1")

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

find_package(Qt5 COMPONENTS Core Network Gui Widgets CONFIG REQUIRED)
find_package(ZenuxCore REQUIRED)

enable_testing()
# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt
function(SETUP_TESTS)
    foreach(_testname ${ARGN})
        add_test(NAME ${_testname}
                 COMMAND ${_testname})
        add_executable(${_testname}
            ${_testname}.h
            ${_testname}.cpp
            )
        target_include_directories(${_testname}
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            )
        target_link_libraries(${_testname}
            PUBLIC
            Qt5::Test
            Zera::zera-timers-testlib
            )
    endforeach()
endfunction()


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    simplecmdiobaseConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file("src/simplecmdiobaseConfig.cmake.in" simplecmdiobaseConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/simplecmdiobaseConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/simplecmdiobaseConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simplecmdiobase
    )

install(EXPORT simplecmdiobase
    FILE simplecmdiobaseTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simplecmdiobase
    )

add_subdirectory(src)
