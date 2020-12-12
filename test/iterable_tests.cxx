#include <si/iterable.hxx>

/**
 * helper to check whether something is an iterator for a specific value type
 */
template<typename T, typename Value>
struct is_iterable {
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
static constexpr bool const is_iterable_v = is_iterable<T, Value>::value;

// test structures to see whether the concept is working as intended
struct int_iterator {
  bool has_next() const;

  int next();
};

struct int_iterable {
  int_iterator iterator() const;
};

struct missing_iterator {
};

// the tests
static_assert(is_iterable_v<int_iterable, int>, "int_iterable should be an si::Iterable<int>");
static_assert(!is_iterable_v<int_iterable, float>, "int_iterable should not be an si::Iterable<float>");
static_assert(!is_iterable_v<missing_iterator, int>, "missing_iterator should not be an si::Iterable<?>");
