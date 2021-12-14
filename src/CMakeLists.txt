add_library(si STATIC
    si/si.hxx
    si/iterator.hxx
    si/iterable.hxx
    si/callables.hxx
    si/algorithms.hxx
    si/algorithms/indexed.hxx
    si/algorithms/map.hxx
    si/generators.hxx
    si/exceptions.hxx si/exceptions.cxx
    si/range.hxx
    )
target_include_directories(si INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
