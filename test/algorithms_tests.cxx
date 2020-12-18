#include <gtest/gtest.h>

#include <si/algorithms.hxx>

#include <string>
#include <vector>

using namespace std::literals::string_literals;

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

  constexpr int next()
  {
    if (!has_next())
      throw si::no_such_element_exception{"Called next() on empty iterator."};
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

TEST(AlgorithmsTests, Transform)
{
  auto const range = IntRange{1, 5};

  std::vector<std::string> results;
  results.reserve(5u);
  std::vector expected{"3"s, "6"s, "9"s, "12"s, "15"s};

  range
      << si::transform<int, int>([](int n) { return n*3; })
      << si::map<int, std::string>([](int n) { return std::to_string(n); })
      << si::for_each<std::string>([&results](std::string&& s) { results.push_back(s); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, Filter)
{
  auto const range = IntRange{1, 5};

  std::vector<int> results;
  results.reserve(3u);
  std::vector expected{6, 8, 10};

  range
      << si::map<int, int>([](int n) { return n*2; })
      << si::filter<int>([](int n) { return n>5; })
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, CopyIf)
{
  auto const range = IntRange{1, 5};

  std::vector<int> results;
  results.reserve(2u);
  std::vector expected{2, 4};

  range
      << si::map<int, int>([](int n) { return n*2; })
      << si::copy_if<int>([](int n) { return n<5; })
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, DropAndTake)
{
  auto const range = IntRange{1, 1'000'000};

  std::vector<int> results;
  results.reserve(5u);
  std::vector expected{6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  range
      << si::drop<int>(5u)
      << si::take<int>(10u)
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, TakeAndDrop)
{
  auto const range = IntRange{1, 1'000'000};

  std::vector<int> results;
  results.reserve(5u);
  std::vector expected{6, 7, 8, 9, 10};

  range
      << si::take<int>(10u)
      << si::drop<int>(5u)
      << si::for_each<int>([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}
