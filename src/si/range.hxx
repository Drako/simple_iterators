#pragma once

#ifndef SI_RANGE_HXX
#define SI_RANGE_HXX

#include "iterable.hxx"
#include "iterator.hxx"

namespace si {
  namespace detail {
    template<Iterator I>
    struct Range {
      I iterator;

      inline auto begin() const
      {
        return IteratorWrapper<I>{iterator};
      }

      inline auto end() const
      {
        return IteratorWrapper<I>{};
      }
    };

    struct ToRange {
    };

    template<Iterable I>
    inline auto operator<<(I const& i, ToRange)
    {
      return Range<decltype(i.iterator())>{i.iterator()};
    }
  }

  inline auto to_range()
  {
    return detail::ToRange{};
  }
}

#endif // SI_RANGE_HXX
