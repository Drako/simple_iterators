# Simple Iterators

This project aims at providing an iterator mechanism inspired by Java.

The iterators of the C++ standard library are rather complex an rely heavily on operator overloading.
This library uses C++ 20 concepts to make anything iterable that has an `iterator()` member function returning
an iterator. An iterator is anything providing the member functions `has_next()` and `next()`.

Next to the two concepts `Iterable<T>` and `Iterator<T>` this library provides a bunch of algorithms
that are compatible with any types implementing these concepts.

## Compiler Support

As of this writing GCC 10 or Clang 10 are required.
See [C++ Compiler Support](https://en.cppreference.com/w/cpp/compiler_support)
for further reference. **Concepts** must be supported.

## Building

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
