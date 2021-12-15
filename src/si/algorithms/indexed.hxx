#pragma once

#ifndef SI_ALGORITHMS_INDEXED_HXX
#define SI_ALGORITHMS_INDEXED_HXX

#include "../iterable.hxx"
#include "./map.hxx"

#include <utility>

namespace si {
  namespace detail {
    struct Indexed final {
    };

    template<Iterable I>
    inline auto operator<<(I const& i, Indexed)
    {
      using T = decltype(i.iterator().next());

      std::size_t idx = 0u;
      auto indexer = [idx](T const& item) mutable { return std::pair{idx++, item}; };
      return i << Map<decltype(indexer)>{indexer};
    }
  }

  inline auto indexed()
  {
    return detail::Indexed{};
  }
}

#endif // SI_ALGORITHMS_INDEXED_HXX
