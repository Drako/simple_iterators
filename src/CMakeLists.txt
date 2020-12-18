add_library(si STATIC
    si/iterator.hxx
    si/iterable.hxx
    si/callables.hxx
    si/algorithms.hxx
    si/exceptions.hxx si/exceptions.cxx
    )
target_include_directories(si INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
