file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_executable(simpleautorun
    ${HEADER}
    ${SOURCES}
    )

target_link_libraries(simpleautorun
    PUBLIC
    Qt5::Core
    simpleautorunlib
    )

install(TARGETS simpleautorun
    EXPORT simpleautorunExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
