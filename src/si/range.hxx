#pragma once

#ifndef SI_RANGE_HXX
#define SI_RANGE_HXX

#include "iterable.hxx"
#include "iterator.hxx"

namespace si {
  namespace detail {
    template<typename T, Iterator <T> I>
    struct Range {
      I iterator;

      inline auto begin() const
      {
        return IteratorWrapper<T, I>{iterator};
      }

      inline auto end() const
      {
        return IteratorWrapper<T, I>{};
      }
    };

    template<typename T>
    struct ToRange {
    };

    template<typename T, Iterable <T> I>
    inline auto operator<<(I const& i, ToRange<T>)
    {
      return Range<T, decltype(i.iterator())>{i.iterator()};
    }
  }

  template<typename ValueType>
  inline auto to_range()
  {
    return detail::ToRange<ValueType>{};
  }
}

#endif // SI_RANGE_HXX
