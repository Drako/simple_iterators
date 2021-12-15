#include <gtest/gtest.h>

#include <si/si.hxx>

#include <array>
#include <string>
#include <unordered_map>

TEST(CollectorTests, CollectIntoStringUntyped)
{
  std::string const abc = si::generate([c = 'A']() mutable {
    return c<='C' ? std::optional{c++} : std::optional<char>{};
  })
      << si::collect();

  EXPECT_EQ(abc, "ABC");
}

TEST(CollectorTests, CollectIntoStringTyped)
{
  auto const abc = si::generate([c = 'A']() mutable {
    return c<='C' ? std::optional{c++} : std::optional<char>{};
  })
      << si::collect<std::string>();

  EXPECT_EQ(abc, "ABC");
}

TEST(CollectorTests, CollectIntoMapUntyped)
{
  std::array<std::string, 3u> const keys{"answer", "illuminati", "leet"};
  std::array<int, 3u> const values{42, 23, 1337};

  std::unordered_map<std::string, int> const expected{
      {"answer", 42},
      {"illuminati", 23},
      {"leet", 1337},
  };

  std::unordered_map<std::string, int> const numbers = si::generate([n = 0]() mutable {
    return n++;
  })
      << si::take(3u)
      << si::map([&keys, &values](int index) {
        return std::make_pair(keys[index], values[index]);
      })
      << si::collect();

  EXPECT_EQ(numbers, expected);
}
