#pragma once

#ifndef SI_ALGORITHMS_FILTER_HXX
#define SI_ALGORITHMS_FILTER_HXX

#include "../exceptions.hxx"
#include "../iterable.hxx"

#include <optional>

namespace si {
  namespace detail {
    template<Iterator I, Predicate P>
    class FilteringIterator final {
      I base;
      using T = decltype(base.next());

      P predicate;

      std::optional<T> next_cached;

      inline void skip()
      {
        while (base.has_next()) {
          auto const value = base.next();
          if (predicate(value)) {
            next_cached = value;
            return;
          }
        }
      }

    public:
      inline FilteringIterator(I const& base, P const& predicate)
          :base{base}, predicate{predicate} { }

      [[nodiscard]] inline bool has_next()
      {
        if (next_cached.has_value()) {
          return true;
        }

        skip();

        return next_cached.has_value();
      }

      inline T next()
      {
        if (!has_next())
          throw no_such_element_exception{"Called next() on empty iterator."};
        auto const result = next_cached.value();
        next_cached.reset();
        return result;
      }
    };

    template<Iterator I, Predicate P>
    struct Filtering final {
      I base;
      P predicate;

      inline auto iterator() const
      {
        return FilteringIterator<I, P>{base, predicate};
      }
    };

    template<Predicate P>
    struct Filter final {
      P predicate;
    };

    template<Iterable I, Predicate P>
    inline auto operator<<(I const& i, Filter<P> const& f)
    {
      return Filtering<decltype(i.iterator()), P>{i.iterator(), f.predicate};
    }
  }

  template<Predicate P>
  inline auto filter(P const& p)
  {
    return detail::Filter<P>{p};
  }

  template<BinaryPredicate BP>
  inline auto filter(BP const& bp)
  {
    using first = typename detail::CallableTraits<BP>::template argument_type<0u>;
    using second = typename detail::CallableTraits<BP>::template argument_type<1u>;
    return filter([bp](std::pair<first, second> const& p) { return bp(p.first, p.second); });
  }
}

#endif // SI_ALGORITHMS_FILTER_HXX
