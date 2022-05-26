#pragma once

#ifndef SI_ALGORITHMS_FIRST_HXX
#define SI_ALGORITHMS_FIRST_HXX

#include "../iterable.hxx"

namespace si {
  namespace detail {
    struct First final {
    };

    template<Iterable I>
    inline auto operator<<(I const& i, First)
    {
      auto it = i.iterator();
      using Return = decltype(it.next());
      return it.has_next() ? std::optional<Return>{it.next()} : std::optional<Return>{};
    }
  }

  inline auto first()
  {
    return detail::First{};
  }
}

#endif // SI_ALGORITHMS_FIRST_HXX
