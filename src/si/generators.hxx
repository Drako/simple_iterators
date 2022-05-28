#pragma once

#ifndef SI_GENERATORS_HXX
#define SI_GENERATORS_HXX

#include <optional>

#include "callables.hxx"
#include "exceptions.hxx"
#include "iterable.hxx"
#include "traits.hxx"

namespace si {
  template<typename T>
  concept Generator = Producer<T>;

  namespace detail {
    template<Generator G>
    class GeneratingIterator final {
      G generator;
      using T = RemoveOptionalT<decltype(generator())>;

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

    template<Generator G>
    struct Generating final {
      G generator;

      auto iterator() const
      {
        return GeneratingIterator<G>{generator};
      }
    };
  }

  template<Generator G>
  inline auto generate(G const& g)
  {
    return detail::Generating<G>{g};
  }
}

#endif // SI_GENERATORS_HXX
