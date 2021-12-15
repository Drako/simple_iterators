#pragma once

#ifndef SI_ALGORITHMS_FOR_EACH_HXX
#define SI_ALGORITHMS_FOR_EACH_HXX

#include "../iterable.hxx"

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
  }

  template<Consumer C>
  inline auto for_each(C const& c)
  {
    return detail::ForEach<C>{c};
  }
}

#endif // SI_ALGORITHMS_FOR_EACH_HXX
