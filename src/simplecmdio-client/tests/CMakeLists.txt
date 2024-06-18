function(SETUP_SIMPLE_PARSER_CLIENT_TESTS)
    foreach(_testname ${ARGN})
        SETUP_TESTS(${_testname})
        target_link_libraries(${_testname}
            PUBLIC
            simplecmdiobase
            simplecmdio-client
            simplecmdio-testlib
        )
    endforeach()
endfunction()

SETUP_SIMPLE_PARSER_CLIENT_TESTS(
    test_task_open
    test_task_simpleio
)
