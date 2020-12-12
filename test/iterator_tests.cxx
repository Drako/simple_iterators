#include <si/iterator.hxx>

/**
 * helper to check whether something is an iterator for a specific value type
 */
template<typename T, typename Value>
struct is_iterator {
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
static constexpr bool const is_iterator_v = is_iterator<T, Value>::value;

// test structures to see whether the concept is working as intended
struct int_iterator {
  bool has_next() const;

  int next();
};

struct missing_next {
  bool has_next() const;
};

struct missing_has_next {
  int next();
};

struct missing_all {
};

// the tests
static_assert(is_iterator_v<int_iterator, int>, "int_iterator should be an si::Iterator<int>");
static_assert(!is_iterator_v<missing_next, int>, "missing_next should not be an si::Iterator<?>");
static_assert(!is_iterator_v<missing_has_next, int>, "missing_has_next should not be an si::Iterator<?>");
static_assert(!is_iterator_v<missing_all, int>, "missing_all should not be an si::Iterator<?>");
static_assert(!is_iterator_v<int_iterator, float>, "int_iterator should not be an si::Iterator<float>");
