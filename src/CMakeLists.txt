add_library(si INTERFACE)
target_sources(si INTERFACE si/iterator.hxx)
target_include_directories(si INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
