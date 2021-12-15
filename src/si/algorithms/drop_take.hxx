#pragma once

#ifndef SI_ALGORITHMS_DROP_TAKE_HXX
#define SI_ALGORITHMS_DROP_TAKE_HXX

#include "../iterable.hxx"

#include <cstddef>
#include <limits>

namespace si {
  namespace detail {
    struct DropTake final {
      std::size_t drop;
      std::size_t take;
    };

    template<Iterator I>
    struct DroppingTakingIterator {
    private:
      I base;
      using T = decltype(base.next());

      DropTake dt;
      unsigned current = 0u;

      inline void skip()
      {
        while (base.has_next() && dt.drop--) {
          (void) base.next();
        }
      }

    public:
      inline DroppingTakingIterator(I const& base, DropTake dt)
          :base{base}, dt{dt}
      {
        skip();
      }

      [[nodiscard]] inline bool has_next()
      {
        return current<dt.take && base.has_next();
      }

      inline T next()
      {
        if (!has_next())
          throw no_such_element_exception{"Called next() on empty iterator."};
        ++current;
        return base.next();
      }
    };

    template<Iterator I>
    struct DroppingTaking final {
      I base;
      using T = decltype(base.next());

      DropTake dt;

      inline auto iterator() const
      {
        return DroppingTakingIterator<I>{base, dt};
      }
    };

    template<Iterable I>
    inline auto operator<<(I const& i, DropTake dt)
    {
      return DroppingTaking<decltype(i.iterator())>{i.iterator(), dt};
    }
  }

  inline auto drop(unsigned n)
  {
    // technically this is not a "drop n, take all" but a "drop n, take SIZE_T_MAX", but that should be sufficient
    return detail::DropTake{n, std::numeric_limits<std::size_t>::max()};
  }

  inline auto take(unsigned n)
  {
    return detail::DropTake{0u, n};
  }
}

#endif // SI_ALGORITHMS_DROP_TAKE_HXX
