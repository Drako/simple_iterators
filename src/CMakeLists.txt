add_library(si INTERFACE)
target_sources(si INTERFACE si/iterator.hxx si/iterable.hxx)
target_include_directories(si INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
