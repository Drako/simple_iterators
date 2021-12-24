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
static_assert(si::Iterator<IntIterator>, "IntIterator should be an si::Iterator<?>");
static_assert(!si::Iterator<MissingNext>, "MissingNext should not be an si::Iterator<?>");
static_assert(!si::Iterator<MissingHasNext>, "MissingHasNext should not be an si::Iterator<?>");
static_assert(!si::Iterator<MissingAll>, "MissingAll should not be an si::Iterator<?>");

TEST(IteratorTests, StringFromIteratorWrapper)
{
  std::string const source{"hello world!"};
  std::string const expected{"HELLO WORLD!"};

  auto const result = si::iterate(source)
      << si::map([](char c) { return std::toupper(c); })
      << si::collect<std::string>();

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

  auto const result = si::generate([n = 1]() mutable { return n++; })
      << si::take(15u)
      << si::map([](int n) {
        std::string result;
        result += (n%3==0) ? "Fizz" : "";
        result += (n%5==0) ? "Buzz" : "";
        return result.empty() ? std::to_string(n) : result;
      })
      << si::collect<std::vector<std::string>>();

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

  constexpr Dummy next() const
  {
    return {42};
  }
};

#ifndef SKIP_EXCEPTION_TESTS
TEST(IteratorTests, TryReadingFromEndIterator)
{
  si::IteratorWrapper<DummyIterator> end{};
  EXPECT_THROW(*end, si::no_such_element_exception);
  EXPECT_THROW((void) end->value, si::no_such_element_exception);
}

TEST(IteratorTests, DereferenceStructure)
{
  DummyIterator it;
  si::IteratorWrapper iw{it};
  EXPECT_EQ((*iw).value, 42);
  EXPECT_EQ(iw->value, 42);
}

#endif // SKIP_EXCEPTION_TESTS
