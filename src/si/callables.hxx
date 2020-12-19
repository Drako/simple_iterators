#pragma once

#ifndef SI_CALLABLES_HXX
#define SI_CALLABLES_HXX

#include <concepts>

namespace si {
  template<typename T, typename Value>
  concept Consumer = requires(T consumer) {
    consumer(std::declval<Value>());
  };

  template<typename T, typename Value>
  concept Producer = requires(T producer) {
    { producer() } -> std::same_as<Value>;
  };

  template<typename T, typename Value>
  concept Predicate = requires(T predicate) {
    { predicate(std::declval<Value>()) } -> std::same_as<bool>;
  };

  template<typename T, typename Source, typename Target>
  concept Mapper = requires(T mapper) {
    { mapper(std::declval<Source>()) } -> std::same_as<Target>;
  };

  template<typename T, typename A, typename B = A, typename Target = A>
  concept Aggregator = requires(T aggregator) {
    { aggregator(std::declval<A>(), std::declval<B>()) } -> std::same_as<Target>;
  };
}

#endif // SI_CALLABLES_HXX
