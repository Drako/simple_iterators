#include <si/iterator.hxx>

/**
 * helper to check whether something is an iterator for a specific value type
 */
template<typename T, typename Value>
struct IsIterator {
  using true_type = char;
  using false_type = struct {
    char dummy[2];
  };

  template<si::Iterator<Value> Iterator>
  static true_type test(Iterator&& it);

  static false_type test(...);

  static constexpr bool const value = sizeof(test(std::declval<T>()))==sizeof(true_type);
};

template<typename T, typename Value>
static constexpr bool const IsIteratorV = IsIterator<T, Value>::value;

// test structures to see whether the concept is working as intended
struct IntIterator {
  bool has_next() const;

  int next();
};

struct MissingNext {
  bool has_next() const;
};

struct MissingHasNext {
  int next();
};

struct MissingAll {
};

// the tests
static_assert(IsIteratorV<IntIterator, int>, "IntIterator should be an si::Iterator<int>");
static_assert(!IsIteratorV<MissingNext, int>, "MissingNext should not be an si::Iterator<?>");
static_assert(!IsIteratorV<MissingHasNext, int>, "MissingHasNext should not be an si::Iterator<?>");
static_assert(!IsIteratorV<MissingAll, int>, "MissingAll should not be an si::Iterator<?>");
static_assert(!IsIteratorV<IntIterator, float>, "IntIterator should not be an si::Iterator<float>");
