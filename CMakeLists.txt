cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)

if (NOT DEFINED PROJECT_NAME)
  set(SI_IS_ROOT ON)
else ()
  set(SI_IS_ROOT OFF)
endif ()

project(simple_iterators)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(contrib)
add_subdirectory(src)

if (SI_IS_ROOT)
  enable_testing()
  add_subdirectory(test)
endif ()
