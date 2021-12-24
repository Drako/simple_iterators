#pragma once

#ifndef SI_ALGORITHMS_COUNT_HXX
#define SI_ALGORITHMS_COUNT_HXX

#include "../iterable.hxx"

#include <cstddef>

namespace si {
  namespace detail {
    struct JustCount final {
    };

    template<Iterable I>
    inline std::size_t operator<<(I const& i, JustCount)
    {
      auto it = i.iterator();
      std::size_t result = 0u;
      while (it.has_next()) {
        (void) it.next();
        ++result;
      }
      return result;
    }

    template<Predicate P>
    struct CountFiltered final {
      P predicate;
    };

    template<Iterable I, Predicate P>
    inline std::size_t operator<<(I const& i, CountFiltered<P> const& cf)
    {
      auto it = i.iterator();
      std::size_t result = 0u;
      while (it.has_next()) {
        if (cf.predicate(it.next()))
          ++result;
      }
      return result;
    }
  }

  inline auto count()
  {
    return detail::JustCount{};
  }

  template<Predicate P>
  inline auto count(P const& p)
  {
    return detail::CountFiltered<P>{p};
  }

  template<BinaryPredicate BP>
  inline auto count(BP const& bp)
  {
    using first = typename detail::CallableTraits<BP>::template argument_type<0u>;
    using second = typename detail::CallableTraits<BP>::template argument_type<1u>;
    return count([bp](std::pair<first, second> const& p) { return bp(p.first, p.second); });
  }
}

#endif // SI_ALGORITHMS_COUNT_HXX
