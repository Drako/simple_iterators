# Simple Iterators

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://github.com/Drako/simple_iterators/workflows/build-and-test/badge.svg)](https://github.com/Drako/simple_iterators/actions?query=workflow:build-and-test)
[![Codecov status](https://codecov.io/gh/Drako/simple_iterators/branch/master/graph/badge.svg)](https://codecov.io/gh/Drako/simple_iterators/branch/master)

This project aims at providing an iterator mechanism inspired by Java.

The iterators of the C++ standard library are rather complex and rely heavily on operator overloading.
This library uses C++ 20 concepts to make anything iterable that has an `iterator()` member function returning
an iterator. An iterator is anything providing the member functions `has_next()` and `next()`.

Next to the two concepts `Iterable` and `Iterator` this library provides a bunch of algorithms
that are compatible with any types implementing these concepts.

## Usage

### Including into Git and CMake based project

Add this library as a submodule and include it via `add_subdirectory()`
into your CMake project.
Then everything you need to do, is to link against the `si` target.

Example:
```shell
git submodule add https://github.com/Drako/simple_iterators
```

Then in your CMakeLists.txt do something like this:
```cmake
add_subdirectory(simple_iterators)

add_executable(hello main.cxx)
target_link_libraries(hello PRIVATE si)
```

### Defining custom iterables

```cpp
/**
 * The minimum required functions are has_next() and next().
 * has_next() is supposed to return a bool.
 */
class IntRangeIterator final {
  int current;
  int last;

public:
  constexpr IntRangeIterator(int start, int last) noexcept
      :current{start}, last{last} { }
      
  [[nodiscard]] constexpr bool has_next() const noexcept
  {
    return current<=last;
  }

  // in case next() is called on an empty iterator, si::no_such_element_exception should be thrown.
  constexpr int next()
  {
    if (!has_next())
      throw si::no_such_element_exception{"Called next() on empty iterator."};
    return current++;
  }
};

/**
 * The minimum required function is iterator().
 */
class IntRange {
  int from;
  int to;

public:
  constexpr IntRange(int from, int to) noexcept
      :from{from}, to{to} { }

  [[nodiscard]] constexpr IntRangeIterator iterator() const noexcept
  {
    return IntRangeIterator{from, to};
  }
};
```

## Compiler Support

As of this writing GCC 10 or Clang 10 are required.
See [C++ Compiler Support](https://en.cppreference.com/w/cpp/compiler_support)
for further reference. **Concepts** must be supported.

## Building & Testing

Other than the aforementioned compilers
the project currently requires [CMake](https://cmake.org/download/) version *3.17* or newer.

Assuming the project has not been cloned yet, this is the further process to get the tests running:

```shell
git clone --recursive https://github.com/Drako/simple_iterators
mkdir -p simple_iterators/build
cd simple_iterators/build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest -V .
```
