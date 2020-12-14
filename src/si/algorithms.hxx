#pragma once

#ifndef SI_ALGORITHMS_HXX
#define SI_ALGORITHMS_HXX

#include "callables.hxx"
#include "iterable.hxx"

namespace si {
  template<typename T, Consumer<T> C>
  struct ForEach {
    C consumer;
  };

  template <typename T, Consumer<T> C>
  inline auto for_each(C &&c) {
    return ForEach<T, C>{c};
  }

  template<typename T, Iterable<T> I, Consumer<T> C>
  void operator<<(I&& i, ForEach<T, C>&& f)
  {
    auto it = i.iterator();
    while (it.has_next()) {
      f.consumer(it.next());
    }
  }
}

#endif // SI_ALGORITHMS_HXX
