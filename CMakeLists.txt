cmake_minimum_required(VERSION 3.14)

project(simple-parser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

#Set project Version
set(SIMPLEPARSER_VERSION_MAJOR "0")
set(SIMPLEPARSER_VERSION_MINOR "3")
set(SIMPLEPARSER_VERSION_PATCH "1")

set(PROJECT_VERSION "${SIMPLEPARSER_VERSION_MAJOR}.${SIMPLEPARSER_VERSION_MINOR}.${SIMPLEPARSER_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

# yes we mean it for all targets
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

#Find dependecies
find_package(Qt5 COMPONENTS Core Network Gui Widgets CONFIG REQUIRED)

add_subdirectory(src)
