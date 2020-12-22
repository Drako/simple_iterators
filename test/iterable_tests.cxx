#include <gtest/gtest.h>

#include <si/si.hxx>

#include <array>
#include <cctype>
#include <functional>
#include <list>
#include <string>
#include <vector>

// test structures to see whether the concept is working as intended
struct IntIterator {
  bool has_next() const;

  int next();
};

struct IntIterable {
  IntIterator iterator() const;
};

struct MissingIterator {
};

// the tests
static_assert(si::Iterable<IntIterable, int>, "IntIterable should be an si::Iterable<int>");
static_assert(!si::Iterable<IntIterable, float>, "IntIterable should not be an si::Iterable<float>");
static_assert(!si::Iterable<MissingIterator, int>, "MissingIterator should not be an si::Iterable<?>");

TEST(IterableTests, LegacyWrapperVector)
{
  std::vector const expected{13, 42, 1337};
  std::vector<int> results;
  results.reserve(3u);

  si::iterate(expected)
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperList)
{
  std::list const expected{13, 42, 1337};
  std::list<int> results;

  si::iterate(expected)
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperArray)
{
  std::array const expected{13, 42, 1337};
  std::array<int, 3u> results{};

  si::iterate(expected)
      << si::indexed<int>()
      << si::for_each<std::pair<std::size_t, int>>([&results](std::pair<std::size_t, int> p) {
        auto const[idx, n] = p;
        results[idx] = n;
      });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperRawArray)
{
  int const source[] = {13, 42, 1337};
  std::array const expected{13, 42, 1337};
  std::array<int, 3u> results{};

  si::iterate(source)
      << si::indexed<int>()
      << si::for_each<std::pair<std::size_t, int>>([&results](std::pair<std::size_t, int> p) {
        auto const[idx, n] = p;
        results[idx] = n;
      });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperPointers)
{
  char const* argv[] = {"alpha", "bravo", "charlie"};
  std::list const expected{"alpha", "bravo", "charlie"};
  std::list<char const*> results;

  si::iterate(+argv, argv+3)
      << si::for_each<char const*>([&results](char const* arg) {
        results.push_back(arg);
      });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperString)
{
  using namespace std::placeholders;

  std::string const source{"hello world!"};
  std::string const expected{"HELLO WORLD!"};
  std::string result;

  si::iterate(source)
      << si::map<char, char>([](char c) -> char { return std::toupper(c); })
      // just to test if it would work... don't actually use std::bind
      << si::for_each<char>(std::bind(&std::string::push_back, &result, _1));

  EXPECT_EQ(result, expected);
}
