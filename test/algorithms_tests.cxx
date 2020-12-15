#include <gtest/gtest.h>

#include <si/algorithms.hxx>

#include <string>
#include <vector>

using namespace std::literals::string_literals;

class IntRangeIterator final {
  int current;
  int last;

public:
  constexpr IntRangeIterator(int current, int last) noexcept
      :current{current}, last{last} { }

  [[nodiscard]] constexpr bool has_next() const noexcept
  {
    return current<=last;
  }

  constexpr int next() noexcept
  {
    return current++;
  }
};

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

TEST(AlgorithmsTests, ForEach)
{
  auto const range = IntRange{1, 10};

  std::vector<int> results(10u);
  std::vector expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  range << si::for_each<int>([&results](int n) {
    results[n-1] = n;
  });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, Map)
{
  auto const range = IntRange{1, 5};

  std::vector<std::string> results;
  results.reserve(5u);
  std::vector expected{"2"s, "4"s, "6"s, "8"s, "10"s};

  range
      << si::map<int, int>([](int n) { return n*2; })
      << si::map<int, std::string>([](int n) { return std::to_string(n); })
      << si::for_each<std::string>([&results](std::string&& s) { results.push_back(s); });

  EXPECT_EQ(results, expected);
}
