#include <gtest/gtest.h>

#include <si/si.hxx>

#include <cctype>
#include <string>
#include <vector>

#include "config.hxx"

// test structures to see whether the concept is working as intended
struct IntIterator {
  bool has_next() const;

  int next();
};

struct MissingNext {
  bool has_next() const;
};

struct MissingHasNext {
  int next();
};

struct MissingAll {
};

// the tests
static_assert(si::Iterator<IntIterator, int>, "IntIterator should be an si::Iterator<int>");
static_assert(!si::Iterator<MissingNext, int>, "MissingNext should not be an si::Iterator<?>");
static_assert(!si::Iterator<MissingHasNext, int>, "MissingHasNext should not be an si::Iterator<?>");
static_assert(!si::Iterator<MissingAll, int>, "MissingAll should not be an si::Iterator<?>");
static_assert(!si::Iterator<IntIterator, float>, "IntIterator should not be an si::Iterator<float>");

TEST(IteratorTests, StringFromIteratorWrapper)
{
  std::string const source{"hello world!"};
  std::string const expected{"HELLO WORLD!"};

  auto mapping = si::iterate(source)
      << si::map<char, char>([](char c) -> char { return std::toupper(c); });

  si::IteratorWrapper<char, decltype(mapping.iterator())> begin{mapping.iterator()};
  si::IteratorWrapper<char, decltype(mapping.iterator())> end{};

  std::string const result{begin, end};

  EXPECT_EQ(result, expected);
}

TEST(IteratorTests, VectorFromIteratorWrapper)
{
  using namespace std::literals::string_literals;

  std::vector const expected{
      "1"s, "2"s, "Fizz"s, "4"s, "Buzz"s,
      "Fizz"s, "7"s, "8"s, "Fizz"s, "Buzz"s,
      "11"s, "Fizz"s, "13"s, "14"s, "FizzBuzz"s,
  };

  auto fizzbuzz = si::generate<int>([n = 1]() mutable -> std::optional<int> { return n++; })
      << si::take<int>(15u)
      << si::map<int, std::string>([](int n) {
        std::string result;
        result += (n%3==0) ? "Fizz" : "";
        result += (n%5==0) ? "Buzz" : "";
        return result.empty() ? std::to_string(n) : result;
      });

  si::IteratorWrapper<std::string, decltype(fizzbuzz.iterator())> begin{fizzbuzz.iterator()};
  si::IteratorWrapper<std::string, decltype(fizzbuzz.iterator())> end{};

  std::vector<std::string> const result{begin, end};

  EXPECT_EQ(result, expected);
}

struct Dummy {
  int value;
};

struct DummyIterator {
  constexpr bool has_next() const
  {
    return true;
  }

  constexpr Dummy next()
  {
    return {42};
  }
};

#ifndef SKIP_EXCEPTION_TESTS
TEST(IteratorTests, TryReadingFromEndIterator)
{
  si::IteratorWrapper<Dummy, DummyIterator> end{};
  EXPECT_THROW(*end, si::no_such_element_exception);
  EXPECT_THROW(end->value, si::no_such_element_exception);
}

TEST(IteratorTests, DereferenceStructure) {
  DummyIterator it;
  si::IteratorWrapper<Dummy, DummyIterator> iw{it};
  EXPECT_EQ((*iw).value, 42);
  EXPECT_EQ(iw->value, 42);
}
#endif // SKIP_EXCEPTION_TESTS
