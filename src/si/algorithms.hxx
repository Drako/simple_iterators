#pragma once

#ifndef SI_ALGORITHMS_HXX
#define SI_ALGORITHMS_HXX

#include "callables.hxx"
#include "exceptions.hxx"
#include "iterable.hxx"

#include <limits>
#include <optional>
#include <utility>

namespace si {
  namespace detail {
    template<typename T, Iterator<T> I>
    inline std::optional<T> maybe_next(I& i)
    {
      if (i.has_next())
        return i.next();
      else
        return {};
    }

    template<typename T, Consumer<T> C>
    struct ForEach final {
      C consumer;
    };

    template<typename T, Iterable<T> I, Consumer<T> C>
    inline void operator<<(I const& i, ForEach<T, C> const& f)
    {
      auto it = i.iterator();
      while (it.has_next()) {
        f.consumer(it.next());
      }
    }

    template<typename Source, typename Target, Iterator<Source> I, Mapper<Source, Target> M>
    struct MappingIterator final {
      I base;
      M mapper;

      [[nodiscard]] inline bool has_next()
      {
        return base.has_next();
      }

      inline Target next()
      {
        return mapper(base.next());
      }
    };

    template<typename Source, typename Target, Iterator<Source> I, Mapper<Source, Target> M>
    struct Mapping final {
      I base;
      M mapper;

      inline auto iterator() const
      {
        return MappingIterator<Source, Target, I, M>{base, mapper};
      }
    };

    template<typename Source, typename Target, Mapper<Source, Target> M>
    struct Map final {
      M mapper;
    };

    template<typename Source, typename Target, Iterable<Source> I, Mapper<Source, Target> M>
    inline auto operator<<(I const& i, Map<Source, Target, M> const& m)
    {
      return Mapping<Source, Target, decltype(i.iterator()), M>{i.iterator(), m.mapper};
    }

    template<typename T, Iterator<T> I, Predicate<T> P>
    class FilteringIterator final {
      I base;
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

    template<typename T, Iterator<T> I, Predicate<T> P>
    struct Filtering final {
      I base;
      P predicate;

      inline auto iterator() const
      {
        return FilteringIterator<T, I, P>{base, predicate};
      }
    };

    template<typename T, Predicate<T> P>
    struct Filter final {
      P predicate;
    };

    template<typename T, Iterable<T> I, Predicate<T> P>
    inline auto operator<<(I const& i, Filter<T, P> const& f)
    {
      return Filtering<T, decltype(i.iterator()), P>{i.iterator(), f.predicate};
    }

    template<typename T>
    struct DropTake final {
      unsigned drop;
      unsigned take;
    };

    template<typename T, Iterator<T> I>
    struct DroppingTakingIterator {
    private:
      I base;
      DropTake<T> dt;
      unsigned current = 0u;

      inline void skip()
      {
        while (base.has_next() && dt.drop--) {
          (void) base.next();
        }
      }

    public:
      inline DroppingTakingIterator(I const& base, DropTake<T> dt)
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

    template<typename T, Iterator<T> I>
    struct DroppingTaking final {
      I base;
      DropTake<T> dt;

      inline auto iterator() const
      {
        return DroppingTakingIterator<T, I>{base, dt};
      }
    };

    template<typename T, Iterable<T> I>
    inline auto operator<<(I const& i, DropTake<T> dt)
    {
      return DroppingTaking<T, decltype(i.iterator())>{i.iterator(), dt};
    }

    template<typename T>
    struct JustCount final {
    };

    template<typename T, Iterable<T> I>
    inline std::size_t operator<<(I const& i, JustCount<T>)
    {
      auto it = i.iterator();
      std::size_t result = 0u;
      while (it.has_next()) {
        (void) it.next();
        ++result;
      }
      return result;
    }

    template<typename T, Predicate<T> P>
    struct CountFiltered final {
      P predicate;
    };

    template<typename T, Iterable<T> I, Predicate<T> P>
    inline std::size_t operator<<(I const& i, CountFiltered<T, P> const& cf)
    {
      auto it = i.iterator();
      std::size_t result = 0u;
      while (it.has_next()) {
        if (cf.predicate(it.next()))
          ++result;
      }
      return result;
    }

    template<typename T>
    struct ZipWithNext final {
    };

    template<typename T, Iterator<T> I>
    class ZippingWithNextIterator final {
      I base;
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
        following = maybe_next<T>(base);
        return result;
      }
    };

    template<typename T, Iterator<T> I>
    struct ZippingWithNext final {
      I base;

      inline auto iterator() const
      {
        return ZippingWithNextIterator<T, I>{base};
      }
    };

    template<typename T, Iterable<T> I>
    inline auto operator<<(I const& i, ZipWithNext<T> const& f)
    {
      return ZippingWithNext<T, decltype(i.iterator())>{i.iterator()};
    }

    template<typename Source, typename Target, Aggregator<Source, Source, Target> A>
    struct ZipWithNextMap final {
      A aggregator;
    };

    template<typename Source, typename Target, Iterable<Source> I, Aggregator<Source, Source, Target> A>
    inline auto operator<<(I const& i, ZipWithNextMap<Source, Target, A> const& a)
    {
      auto const mapper = [a](std::pair<Source, Source> p) { return a.aggregator(p.first, p.second); };
      return i << ZipWithNext<Source>{} << Map<std::pair<Source, Source>, Target, decltype(mapper)>{mapper};
    }

    template<typename T>
    struct Indexed final {
    };

    template<typename T, Iterable<T> I>
    inline auto operator<<(I const& i, Indexed<T>)
    {
      std::size_t idx = 0u;
      auto indexer = [idx](T const& item) mutable { return std::pair{idx++, item}; };
      return i << Map<T, std::pair<std::size_t, T>, decltype(indexer)>{indexer};
    }
  }

  template<typename T, Consumer<T> C>
  inline auto for_each(C const& c)
  {
    return detail::ForEach<T, C>{c};
  }

  template<typename Source, typename Target, Mapper<Source, Target> M>
  inline auto map(M const& m)
  {
    return detail::Map<Source, Target, M>{m};
  }

  template<typename Source, typename Target, Mapper<Source, Target> M>
  inline auto transform(M const& m)
  {
    return detail::Map<Source, Target, M>{m};
  }

  template<typename T, Predicate<T> P>
  inline auto filter(P const& p)
  {
    return detail::Filter<T, P>{p};
  }

  template<typename T, Predicate<T> P>
  inline auto copy_if(P const& p)
  {
    return detail::Filter<T, P>{p};
  }

  template<typename T>
  inline auto drop(unsigned n)
  {
    // technically this is not a "drop n, take all" but a "drop n, take 4.x billion", but that should be sufficient
    return detail::DropTake<T>{n, std::numeric_limits<unsigned>::max()};
  }

  template<typename T>
  inline auto take(unsigned n)
  {
    return detail::DropTake<T>{0u, n};
  }

  template<typename T>
  inline auto count()
  {
    return detail::JustCount<T>{};
  }

  template<typename T, Predicate<T> P>
  inline auto count(P const& p)
  {
    return detail::CountFiltered<T, P>{p};
  }

  template<typename T>
  inline auto zip_with_next()
  {
    return detail::ZipWithNext<T>{};
  }

  template<typename Source, typename Target, Aggregator<Source, Source, Target> A>
  inline auto zip_with_next(A const& agg)
  {
    return detail::ZipWithNextMap<Source, Target, A>{agg};
  }

  template<typename T>
  inline auto indexed()
  {
    return detail::Indexed<T>{};
  }
}

#endif // SI_ALGORITHMS_HXX
