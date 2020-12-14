#include <si/iterable.hxx>

/**
 * helper to check whether something is an iterator for a specific value type
 */
template<typename T, typename Value>
struct IsIterable {
  using true_type = char;
  using false_type = struct {
    char dummy[2];
  };

  template<si::Iterable<Value> Iterable>
  static true_type test(Iterable&& it);

  static false_type test(...);

  static constexpr bool const value = sizeof(test(std::declval<T>()))==sizeof(true_type);
};

template<typename T, typename Value>
static constexpr bool const IsIterableV = IsIterable<T, Value>::value;

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
static_assert(IsIterableV<IntIterable, int>, "IntIterable should be an si::Iterable<int>");
static_assert(!IsIterableV<IntIterable, float>, "IntIterable should not be an si::Iterable<float>");
static_assert(!IsIterableV<MissingIterator, int>, "MissingIterator should not be an si::Iterable<?>");
