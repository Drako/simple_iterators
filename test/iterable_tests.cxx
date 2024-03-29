#include <gtest/gtest.h>

#include <si/si.hxx>

#include <array>
#include <cctype>
#include <functional>
#include <list>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

#include "config.hxx"

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
static_assert(si::Iterable<IntIterable>, "IntIterable should be an si::Iterable<?>");
static_assert(!si::Iterable<MissingIterator>, "MissingIterator should not be an si::Iterable<?>");

TEST(IterableTests, LegacyWrapperVector)
{
  std::vector const expected{13, 42, 1337};
  std::vector<int> results;
  results.reserve(3u);

  si::iterate(expected)
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperList)
{
  std::list const expected{13, 42, 1337};
  std::list<int> results;

  si::iterate(expected)
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperArray)
{
  std::array const expected{13, 42, 1337};
  std::array<int, 3u> results{};

  si::iterate(expected)
      << si::indexed()
      << si::for_each([&results](std::pair<std::size_t, int> p) {
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
      << si::indexed()
      << si::for_each([&results](std::size_t idx, int n) {
        results[idx] = n;
      });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperPointers)
{
  char const* argv[] = {"alpha", "bravo", "charlie"};
  std::list<char const*> const expected{argv+0, argv+3};
  std::list<char const*> results;

  si::iterate(+argv, argv+3)
      << si::for_each([&results](char const* arg) {
        results.push_back(arg);
      });

  EXPECT_EQ(results, expected);
}

TEST(IterableTests, LegacyWrapperString)
{
  using namespace std::placeholders;

  std::string const source{"hello world!"};
  std::string const expected{"HELLO WORLD!"};
  std::string const result = si::iterate(source)
      << si::map([](char c) { return std::toupper(c); })
      << si::collect();

  EXPECT_EQ(result, expected);
}

TEST(IterableTests, LegacyWrapperUnorderedMap)
{
  std::unordered_map<std::string, int> const source{
      {"answer",     42},
      {"illuminati", 23},
      {"leet",       1337},
  };
  using kv = decltype(source)::value_type;

  std::set const expected{42, 23, 1337};
  std::set<int> const result = si::iterate(source)
      << si::map_field(&kv::second)
      << si::collect();

  EXPECT_EQ(result, expected);
}

#ifndef SKIP_EXCEPTION_TESTS
TEST(IterableTests, LegacyWrapperReadAfterEnd)
{
  std::vector<int> const empty;
  auto const iterable = si::iterate(empty);
  auto it = iterable.iterator();
  EXPECT_THROW(it.next(), si::no_such_element_exception);
}

#endif // SKIP_EXCEPTION_TESTS
