set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    cmdparserfile.h
    cmdhandlerfile.h
    commonhelpers.h
)

set(SOURCES
    main.cpp
    cmdparserfile.cpp
    cmdhandlerfile.cpp
    commonhelpers.cpp
)

add_executable(simpleautorun
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries (simpleautorun
    PUBLIC
    simplecmdiobase
)

install(TARGETS simpleautorun
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

