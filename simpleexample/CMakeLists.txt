set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    cmdparserexample.h
    cmdhandlerexample.h
)

set(SOURCES
    main.cpp
    cmdparserexample.cpp
    cmdhandlerexample.cpp
)

add_executable(simpleexample
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries (simpleexample
    PUBLIC
    simplecmdiobase
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

install(TARGETS simpleexample
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
