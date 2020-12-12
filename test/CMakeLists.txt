add_executable(si_tests iterator_tests.cxx)
target_link_libraries(si_tests PRIVATE si gtest gmock gmock_main)

add_test(NAME si_tests COMMAND si_tests)
