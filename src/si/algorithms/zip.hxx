#pragma once

#ifndef SI_ALGORITHMS_ZIP_HXX
#define SI_ALGORITHMS_ZIP_HXX

#include "../iterable.hxx"
#include "./map.hxx"

#include <optional>
#include <utility>

namespace si {
  namespace detail {
    struct ZipWithNext final {
    };

    template<Iterator I>
    class ZippingWithNextIterator final {
      I base;
      using T = decltype(base.next());

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
        following = maybe_next(base);
        return result;
      }
    };

    template<Iterator I>
    struct ZippingWithNext final {
      I base;

      inline auto iterator() const
      {
        return ZippingWithNextIterator<I>{base};
      }
    };

    template<Iterable I>
    inline auto operator<<(I const& i, ZipWithNext const& f)
    {
      return ZippingWithNext<decltype(i.iterator())>{i.iterator()};
    }

    template<Aggregator A>
    struct ZipWithNextMap final {
      A aggregator;
    };

    template<Iterable I, BinaryMapper BM>
    inline auto operator<<(I const& i, ZipWithNextMap<BM> const& bm)
    {
      using Source = decltype(i.iterator().next());

      auto const mapper = [bm](std::pair<Source, Source> p) { return bm.aggregator(p.first, p.second); };
      return i << ZipWithNext{} << Map<decltype(mapper)>{mapper};
    }
  }

  inline auto zip_with_next()
  {
    return detail::ZipWithNext{};
  }

  template<Aggregator A>
  inline auto zip_with_next(A const& agg)
  {
    return detail::ZipWithNextMap<A>{agg};
  }
}

#endif // SI_ALGORITHMS_ZIP_HXX
