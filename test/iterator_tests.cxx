#include <si/iterator.hxx>

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
static_assert(si::Iterator<IntIterator, int>, "IntIterator should be an si::Iterator<int>");
static_assert(!si::Iterator<MissingNext, int>, "MissingNext should not be an si::Iterator<?>");
static_assert(!si::Iterator<MissingHasNext, int>, "MissingHasNext should not be an si::Iterator<?>");
static_assert(!si::Iterator<MissingAll, int>, "MissingAll should not be an si::Iterator<?>");
static_assert(!si::Iterator<IntIterator, float>, "IntIterator should not be an si::Iterator<float>");
