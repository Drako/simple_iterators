#pragma once

#ifndef SI_GENERATORS_HXX
#define SI_GENERATORS_HXX

#include <optional>

#include "callables.hxx"
#include "exceptions.hxx"
#include "iterable.hxx"

namespace si {
  template<typename T, typename Value>
  concept Generator = Producer<T, std::optional<Value>>;

  namespace detail {
    template<typename T, Generator<T> G>
    class GeneratingIterator final {
      G generator;
      std::optional<T> value;

    public:
      inline explicit GeneratingIterator(G const& gen)
          :generator{gen}
      {
        value = generator();
      }

      inline bool has_next() const
      {
        return value.has_value();
      }

      inline T next()
      {
        if (!has_next())
          throw no_such_element_exception{"Called next() on finished generator."};

        auto const result = value.value();
        value = generator();
        return result;
      }
    };

    template<typename T, Generator<T> G>
    struct Generating final {
      G generator;

      auto iterator()
      {
        return GeneratingIterator<T, G>{generator};
      }
    };
  }

  template<typename T, Generator<T> G>
  inline auto generate(G const& g)
  {
    return detail::Generating<T, G>{g};
  }
}

#endif // SI_GENERATORS_HXX
