#pragma once

#ifndef SI_ALGORITHMS_HXX
#define SI_ALGORITHMS_HXX

#include "callables.hxx"
#include "iterable.hxx"

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

      inline Target next() { return mapper(base.next()); }
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
    struct FilteringIterator final {
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

  template<typename T, Predicate<T> P>
  inline auto filter(P&& p)
  {
    return detail::Filter<T, P>{p};
  }
}

#endif // SI_ALGORITHMS_HXX