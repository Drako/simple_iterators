#pragma once

#ifndef SI_ITERATOR_HXX
#define SI_ITERATOR_HXX

#include <concepts>
#include <iterator>
#include <optional>

#include "callables.hxx"
#include "exceptions.hxx"

namespace si {
  template<typename T>
  concept Iterator = requires(T iterator) {
    { iterator.has_next() } -> std::same_as<bool>;
    { iterator.next() } -> NotVoid;
  };

  template<Iterator I>
  inline auto maybe_next(I& i) -> std::optional<decltype(i.next())>
  {
    if (i.has_next())
      return i.next();
    else
      return {};
  }

  template<Iterator I>
  class IteratorWrapper final {
    mutable std::optional<I> base;

    using T = decltype(std::declval<I>().next());
    std::optional<T> value{};

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T const*;
    using reference = T const&;

    inline IteratorWrapper()
        :base{} { }

    explicit inline IteratorWrapper(I const& base)
        :base{base}
    {
      auto& it = this->base.value();
      if (it.has_next()) {
        value = it.next();
      }
    }

    inline bool operator!=(IteratorWrapper const& rhs) const
    {
      return value.has_value() ||
          rhs.value.has_value() ||
          (base.has_value() && base.value().has_next()) ||
          (rhs.base.has_value() && rhs.base.value().has_next());
    }

    inline bool operator==(IteratorWrapper const& rhs) const
    {
      return !operator!=(rhs);
    }

    inline pointer operator->() const
    {
      if (!value.has_value()) {
        throw no_such_element_exception{"Tried dereferencing end iterator."};
      }

      return std::addressof(value.value());
    }

    inline reference operator*() const
    {
      if (!value.has_value()) {
        throw no_such_element_exception{"Tried dereferencing end iterator."};
      }

      return value.value();
    }

    inline IteratorWrapper& operator++()
    {
      if (base.has_value()) {
        value = maybe_next(base.value());
      }
      return *this;
    }

    inline IteratorWrapper operator++(int)
    {
      IteratorWrapper const old{*this};
      ++(*this);
      return old;
    }
  };
}

#endif // SI_ITERATOR_HXX
