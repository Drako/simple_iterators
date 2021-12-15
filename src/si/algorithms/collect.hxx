#pragma once

#ifndef SI_ALGORITHMS_COLLECT_HXX
#define SI_ALGORITHMS_COLLECT_HXX

#include "../iterable.hxx"

namespace si {
  namespace detail {
    struct UntypedCollect final {
    };

    template<Iterable I>
    class UntypedCollector final {
      I iterable;

    public:
      explicit UntypedCollector(I const& i)
          :iterable{i}
      {
      }

      template<typename Container>
      operator Container() const
      {
        using Iterator = decltype(iterable.iterator());
        return Container{IteratorWrapper{iterable.iterator()}, IteratorWrapper<Iterator>{}};
      }
    };

    template<Iterable I>
    inline UntypedCollector<I> operator<<(I const& i, UntypedCollect)
    {
      return UntypedCollector{i};
    }

    template<typename Container>
    struct TypedCollect final {
    };

    template<Iterable I, typename Container>
    inline Container operator<<(I const& i, TypedCollect<Container>)
    {
      using Iterator = decltype(i.iterator());
      return Container{IteratorWrapper{i.iterator()}, IteratorWrapper<Iterator>{}};
    }
  }

  template<typename Container>
  inline auto collect()
  {
    return detail::TypedCollect<Container>{};
  }

  inline auto collect()
  {
    return detail::UntypedCollect{};
  }
}

#endif // SI_ALGORITHMS_COLLECT_HXX
