#pragma once

#ifndef SI_ITERATOR_HXX
#define SI_ITERATOR_HXX

#include <concepts>

namespace si {
  template <typename T, typename Value>
  concept Iterator = requires(T iterator) {
    { iterator.has_next() } -> std::same_as<bool>;
    { iterator.next() } -> std::same_as<Value>;
  };
}

#endif // SI_ITERATOR_HXX
