add_executable(si_tests
    iterator_tests.cxx
    iterable_tests.cxx
    algorithms_tests.cxx
    generators_tests.cxx
    range_tests.cxx
    collector_tests.cxx)
target_link_libraries(si_tests PRIVATE si gtest gmock gmock_main)

add_test(NAME si_tests COMMAND si_tests)
