#pragma once

#ifndef SI_ALGORITHMS_HXX
#define SI_ALGORITHMS_HXX

#include "callables.hxx"
#include "exceptions.hxx"
#include "iterable.hxx"

#include <limits>
#include <optional>

namespace si {
  namespace detail {
    template<typename T, Consumer<T> C>
    struct ForEach final {
      C consumer;
    };

    template<typename T, Iterable<T> I, Consumer<T> C>
    inline void operator<<(I&& i, ForEach<T, C>&& f)
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

      [[nodiscard]] inline bool has_next() const { return base.has_next(); }

      inline Target next()
      {
        return mapper(base.next());
      }
    };

    template<typename Source, typename Target, Iterator<Source> I, Mapper<Source, Target> M>
    struct Mapping final {
      I base;
      M mapper;

      inline auto iterator()
      {
        return MappingIterator<Source, Target, I, M>{base, mapper};
      }
    };

    template<typename Source, typename Target, Mapper<Source, Target> M>
    struct Map final {
      M mapper;
    };

    template<typename Source, typename Target, Iterable<Source> I, Mapper<Source, Target> M>
    inline auto operator<<(I&& i, Map<Source, Target, M>&& m)
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

      inline auto iterator()
      {
        return FilteringIterator<T, I, P>{base, predicate};
      }
    };

    template<typename T, Predicate<T> P>
    struct Filter final {
      P predicate;
    };

    template<typename T, Iterable<T> I, Predicate<T> P>
    inline auto operator<<(I&& i, Filter<T, P>&& f)
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

      [[nodiscard]] inline bool has_next() const
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

      inline auto iterator()
      {
        return DroppingTakingIterator<T, I>{base, dt};
      }
    };

    template<typename T, Iterable<T> I>
    inline auto operator<<(I&& i, DropTake<T> dt)
    {
      return DroppingTaking<T, decltype(i.iterator())>{i.iterator(), dt};
    }
  }

  template<typename T, Consumer<T> C>
  inline auto for_each(C&& c)
  {
    return detail::ForEach<T, C>{c};
  }

  template<typename Source, typename Target, Mapper<Source, Target> M>
  inline auto map(M&& m)
  {
    return detail::Map<Source, Target, M>{m};
  }

  template<typename Source, typename Target, Mapper<Source, Target> M>
  inline auto transform(M&& m)
  {
    return detail::Map<Source, Target, M>{m};
  }

  template<typename T, Predicate<T> P>
  inline auto filter(P&& p)
  {
    return detail::Filter<T, P>{p};
  }

  template<typename T, Predicate<T> P>
  inline auto copy_if(P&& p)
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
}

#endif // SI_ALGORITHMS_HXX
