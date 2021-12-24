#include <gtest/gtest.h>

#include <si/algorithms.hxx>

#include <string>
#include <vector>

#include "config.hxx"

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

#ifndef SKIP_EXCEPTION_TESTS
TEST(AlgorithmsTests, TryReadingAfterEndOfIntRangeIterator)
{
  auto const range = IntRange{0, 1};
  auto it = range.iterator();
  EXPECT_EQ(it.next(), 0);
  EXPECT_EQ(it.next(), 1);
  EXPECT_THROW(it.next(), si::no_such_element_exception);
}

TEST(AlgorithmsTests, TryReadingAfterEndOfFilteringIterator)
{
  auto const range = IntRange{0, 1}
      << si::filter([](int n) { return n>2; });
  auto it = range.iterator();
  EXPECT_THROW(it.next(), si::no_such_element_exception);
}

TEST(AlgorithmsTests, TryReadingAfterEndOfDroppingTakingIterator)
{
  auto const range = IntRange{0, 1}
      << si::drop(2u);
  auto it = range.iterator();
  EXPECT_THROW(it.next(), si::no_such_element_exception);
}

#endif // SKIP_EXCEPTION_TESTS

TEST(AlgorithmsTests, ForEach)
{
  auto const range = IntRange{1, 10};

  std::vector<int> results(10u);
  std::vector expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  range << si::for_each([&results](int n) {
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
      << si::map([](int n) { return n*2; })
      << si::map([](int n) { return std::to_string(n); })
      << si::for_each([&results](std::string&& s) { results.push_back(s); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, Filter)
{
  auto const range = IntRange{1, 5};

  std::vector<int> results;
  results.reserve(3u);
  std::vector expected{6, 8, 10};

  range
      << si::map([](int n) { return n*2; })
      << si::filter([](int n) { return n>5; })
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, BinaryFilter)
{
  std::vector<int> const numbers{23, 42, 1337};
  std::vector<std::size_t> const expected{0u, 2u};
  std::vector<std::size_t> const indexes_of_odd_numbers = si::iterate(numbers)
      << si::indexed()
      << si::filter([](std::size_t, int const n) { return (n & 1)==1; })
      << si::map_field(&std::pair<std::size_t, int>::first)
      << si::collect();

  EXPECT_EQ(indexes_of_odd_numbers, expected);
}

TEST(AlgorithmsTests, DropAndTake)
{
  auto const range = IntRange{1, 1'000'000};

  std::vector<int> results;
  results.reserve(5u);
  std::vector expected{6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  range
      << si::drop(5u)
      << si::take(10u)
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, TakeAndDrop)
{
  auto const range = IntRange{1, 1'000'000};

  std::vector<int> results;
  results.reserve(5u);
  std::vector expected{6, 7, 8, 9, 10};

  range
      << si::take(10u)
      << si::drop(5u)
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, Count)
{
  auto const range = IntRange{1, 5};
  EXPECT_EQ(range << si::count(), 5);
}

TEST(AlgorithmsTests, CountWithPredicate)
{
  auto const range = IntRange{1, 20};
  auto const count = range << si::count([](int n) { return n%2==0; });
  EXPECT_EQ(count, 10);
}

TEST(AlgorithmsTests, ZipWithNext)
{
  std::vector<std::pair<int, int>> const expected{{1, 2},
                                                  {2, 3},
                                                  {3, 4}};
  std::vector<std::pair<int, int>> const results = IntRange{1, 4}
      << si::zip_with_next()
      << si::collect();

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, ZipWithNextOne)
{
  std::vector<std::pair<int, int>> const expected{};
  std::vector<std::pair<int, int>> results;

  IntRange{1, 1}
      << si::zip_with_next()
      << si::for_each([&results](std::pair<int, int> p) { results.push_back(p); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, ZipWithNextEmpty)
{
  std::vector<std::pair<int, int>> const expected{};
  std::vector<std::pair<int, int>> results;

  IntRange{1, 0}
      << si::zip_with_next()
      << si::for_each([&results](std::pair<int, int> p) { results.push_back(p); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, ZipWithNextMap)
{
  std::vector const expected{3, 5, 7};
  std::vector<int> results;

  IntRange{1, 4}
      << si::zip_with_next([](int a, int b) { return a+b; })
      << si::for_each([&results](int n) { results.push_back(n); });

  EXPECT_EQ(results, expected);
}

TEST(AlgorithmsTests, Indexed)
{
  std::vector<std::pair<std::size_t, int>> const expected{{0u, 10},
                                                          {1u, 12},
                                                          {2u, 14}};
  std::vector<std::pair<std::size_t, int>> results;
  results.reserve(3u);

  IntRange{5, 20}
      << si::filter([](int n) { return n%2==0; })
      << si::drop(2u)
      << si::take(3u)
      << si::indexed()
      << si::for_each([&results](std::pair<std::size_t, int> p) {
        auto const[idx, n] = p;
        results.emplace_back(idx, n);
      });

  EXPECT_EQ(results, expected);
}
