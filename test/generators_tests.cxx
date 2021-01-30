#include <gtest/gtest.h>

#include <si/si.hxx>

#include <vector>

#include "config.hxx"

namespace {
  auto int_range(int from, int to)
  {
    return si::generate<int>([=]() mutable -> std::optional<int> {
      if (from>to) return {};
      return from++;
    });
  }
}

TEST(GeneratorsTests, GenerateInfinite)
{
  auto fib = si::generate<int>([a = 0, b = 1]() mutable -> std::optional<int> {
    auto const result = a;
    a = b;
    b += result;
    return result;
  });

  auto const greaterThanTwenty = fib
      << si::take<int>(20u)
      << si::count<int>([](int n) { return n>20; });

  EXPECT_EQ(greaterThanTwenty, 12);
}

TEST(GeneratorsTests, GenerateFinite)
{
  std::vector const expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> results;
  results.reserve(10u);

  int_range(0, 9)
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

#ifndef SKIP_EXCEPTION_TESTS
TEST(GeneratorsTests, TryToGetMoreValuesThanGenerated)
{
  auto gen = int_range(0, 1);
  auto it = gen.iterator();
  EXPECT_EQ(it.next(), 0);
  EXPECT_EQ(it.next(), 1);
  EXPECT_THROW(it.next(), si::no_such_element_exception);
}

#endif // SKIP_EXCEPTION_TESTS
