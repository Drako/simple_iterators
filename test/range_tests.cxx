#include <gtest/gtest.h>

#include <si/si.hxx>

#include <vector>

#include "config.hxx"

namespace {
  auto int_range(int from, int to)
  {
    return si::generate([=]() mutable -> std::optional<int> {
      if (from>to) return {};
      return from++;
    });
  }
}

TEST(RangeTests, RangeBasedFor)
{
  int n = 0;
  for (auto m: int_range(0, 10) << si::to_range()) {
    EXPECT_EQ(n++, m);
  }
}

TEST(RangeTests, Recursion)
{
  std::vector const expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<int> results;
  results.reserve(expected.size());

  si::iterate(si::iterate(int_range(1, 10) << si::to_range()) << si::to_range())
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(expected, results);
}
