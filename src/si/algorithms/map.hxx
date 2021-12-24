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

  template<BinaryMapper BM>
  inline auto map(BM const& bm)
  {
    using first = typename detail::CallableTraits<BM>::template argument_type<0u>;
    using second = typename detail::CallableTraits<BM>::template argument_type<1u>;
    return map([bm](std::pair<first, second> const& p) { return bm(p.first, p.second); });
  }

  template<typename Class, typename Member>
  inline auto map_field(Member Class::* p)
  {
    return map([p](Class const& instance) { return instance.*p; });
  }
}

#endif // SI_ALGORITHMS_MAP_HXX
