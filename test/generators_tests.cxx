#include <gtest/gtest.h>

#include <si/si.hxx>

#include <vector>

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
  auto range = si::generate<int>([n = 0]() mutable -> std::optional<int> {
    if (n==10) return {};
    return n++;
  });

  std::vector const expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> results;
  results.reserve(10u);

  range
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}
