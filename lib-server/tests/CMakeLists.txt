function(SETUP_SIMPLE_PARSER_SERVER_TESTS)
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

SETUP_SIMPLE_PARSER_SERVER_TESTS(
    test_testcompleteserver
)

