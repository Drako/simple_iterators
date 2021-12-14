#pragma once

#ifndef SI_ITERABLE_HXX
#define SI_ITERABLE_HXX

#include "exceptions.hxx"
#include "iterator.hxx"

#include <iterator>
#include <type_traits>

namespace si {
  template<typename T>
  concept Iterable = requires(T iterable) {
    { iterable.iterator() } -> si::Iterator;
  };

  template<typename T>
  concept LegacyIterable = requires(T const& iterable) {
    { std::cbegin(iterable) };
    { std::cend(iterable) };
  };

  namespace detail {
    template<typename LegacyIterator>
    class LegacyIterableWrappingIterator final {
      using ValueType = typename std::iterator_traits<LegacyIterator>::value_type;

      LegacyIterator begin;
      LegacyIterator end;

    public:
      inline LegacyIterableWrappingIterator(LegacyIterator begin, LegacyIterator end)
          :begin{begin}, end{end} { }

      [[nodiscard]] inline bool has_next() const
      {
        return begin!=end;
      }

      inline ValueType next()
      {
        if (!has_next())
          throw si::no_such_element_exception{"Called next() on legacy end iterator."};

        return *begin++;
      }
    };

    template<typename LegacyIterator>
    class LegacyIterableWrapper final {
      LegacyIterator begin;
      LegacyIterator end;

    public:
      inline LegacyIterableWrapper(LegacyIterator begin, LegacyIterator end)
          :begin{begin}, end{end} { }

      inline auto iterator() const
      {
        return LegacyIterableWrappingIterator{begin, end};
      }
    };
  }

  template<typename Iterator>
  inline auto iterate(Iterator const& begin, Iterator const& end)
  {
    return detail::LegacyIterableWrapper{begin, end};
  }

  template<LegacyIterable LI>
  inline auto iterate(LI const& li)
  {
    return iterate(std::cbegin(li), std::cend(li));
  }
}

#endif // SI_ITERABLE_HXX
