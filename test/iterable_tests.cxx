#include <si/iterable.hxx>

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
static_assert(si::Iterable<IntIterable, int>, "IntIterable should be an si::Iterable<int>");
static_assert(!si::Iterable<IntIterable, float>, "IntIterable should not be an si::Iterable<float>");
static_assert(!si::Iterable<MissingIterator, int>, "MissingIterator should not be an si::Iterable<?>");
