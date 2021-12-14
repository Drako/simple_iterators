#pragma once

#ifndef SI_ALGORITHMS_MAP_HXX
#define SI_ALGORITHMS_MAP_HXX

#include "../callables.hxx"
#include "../iterable.hxx"

namespace si {
  namespace detail {
    template<Iterator I, Mapper M>
    struct MappingIterator final {
      I base;
      M mapper;

      [[nodiscard]] inline bool has_next()
      {
        return base.has_next();
      }

      inline auto next()
      {
        return mapper(base.next());
      }
    };

    template<Iterator I, Mapper M>
    struct Mapping final {
      I base;
      M mapper;

      inline auto iterator() const
      {
        return MappingIterator<I, M>{base, mapper};
      }
    };

    template<Mapper M>
    struct Map final {
      M mapper;
    };

    template<Iterable I, Mapper M>
    inline auto operator<<(I const& i, Map<M> const& m)
    {
      return Mapping<decltype(i.iterator()), M>{i.iterator(), m.mapper};
    }
  }

  template<Mapper M>
  inline auto map(M const& m)
  {
    return detail::Map<M>{m};
  }

  template<Mapper M>
  inline auto transform(M const& m)
  {
    return detail::Map<M>{m};
  }
}

#endif // SI_ALGORITHMS_MAP_HXX
