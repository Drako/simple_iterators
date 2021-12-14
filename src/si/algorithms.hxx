#pragma once

#ifndef SI_ALGORITHMS_HXX
#define SI_ALGORITHMS_HXX

#include "callables.hxx"
#include "exceptions.hxx"
#include "iterable.hxx"

#include "algorithms/map.hxx"
#include "algorithms/indexed.hxx"

#include <limits>
#include <optional>
#include <utility>

namespace si {
  namespace detail {
    template<Consumer C>
    struct ForEach final {
      C consumer;
    };

    template<Iterable I, Consumer C>
    inline void operator<<(I const& i, ForEach<C> const& f)
    {
      auto it = i.iterator();
      while (it.has_next()) {
        f.consumer(it.next());
      }
    }

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

    struct DropTake final {
      unsigned drop;
      unsigned take;
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

    struct ZipWithNext final {
    };

    template<Iterator I>
    class ZippingWithNextIterator final {
      I base;
      using T = decltype(base.next());

      std::optional<T> current;
      std::optional<T> following;

    public:
      inline explicit ZippingWithNextIterator(I const& base)
          :base{base}
      {
        if (this->base.has_next()) {
          current = this->base.next();
          if (this->base.has_next()) {
            following = this->base.next();
          }
        }
      }

      [[nodiscard]] inline bool has_next() const noexcept
      {
        return following.has_value();
      }

      inline std::pair<T, T> next()
      {
        std::pair result{current.value(), following.value()};
        current = following;
        following = maybe_next(base);
        return result;
      }
    };

    template<Iterator I>
    struct ZippingWithNext final {
      I base;

      inline auto iterator() const
      {
        return ZippingWithNextIterator<I>{base};
      }
    };

    template<Iterable I>
    inline auto operator<<(I const& i, ZipWithNext const& f)
    {
      return ZippingWithNext<decltype(i.iterator())>{i.iterator()};
    }

    template<Aggregator A>
    struct ZipWithNextMap final {
      A aggregator;
    };

    template<Iterable I, Aggregator A>
    inline auto operator<<(I const& i, ZipWithNextMap<A> const& a)
    {
      using Source = decltype(i.iterator().next());

      auto const mapper = [a](std::pair<Source, Source> p) { return a.aggregator(p.first, p.second); };
      return i << ZipWithNext{} << Map<decltype(mapper)>{mapper};
    }
  }

  template<Consumer C>
  inline auto for_each(C const& c)
  {
    return detail::ForEach<C>{c};
  }

  template<Predicate P>
  inline auto filter(P const& p)
  {
    return detail::Filter<P>{p};
  }

  template<Predicate P>
  inline auto copy_if(P const& p)
  {
    return detail::Filter<P>{p};
  }

  inline auto drop(unsigned n)
  {
    // technically this is not a "drop n, take all" but a "drop n, take 4.x billion", but that should be sufficient
    return detail::DropTake{n, std::numeric_limits<unsigned>::max()};
  }

  inline auto take(unsigned n)
  {
    return detail::DropTake{0u, n};
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

  inline auto zip_with_next()
  {
    return detail::ZipWithNext{};
  }

  template<Aggregator A>
  inline auto zip_with_next(A const& agg)
  {
    return detail::ZipWithNextMap<A>{agg};
  }
}

#endif // SI_ALGORITHMS_HXX
