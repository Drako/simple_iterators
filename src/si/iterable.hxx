#pragma once

#ifndef SI_ITERABLE_HXX
#define SI_ITERABLE_HXX

#include "iterator.hxx"

namespace si {
  template <typename T, typename Value>
  concept Iterable = requires(T iterable) {
    { iterable.iterator() } -> si::Iterator<Value>;
  };
}

#endif // SI_ITERABLE_HXX
