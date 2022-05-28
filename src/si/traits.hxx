#pragma once

#ifndef SI_TRAITS_HXX
#define SI_TRAITS_HXX

#include <optional>
#include <type_traits>

namespace si::detail {
  template<typename T>
  struct RemoveOptional final {
    using type = T;
  };

  template<typename T>
  struct RemoveOptional<std::optional<T>> final {
    using type = T;
  };

  template<typename T>
  using RemoveOptionalT = typename RemoveOptional<T>::type;

  template<typename T>
  struct IsOptional final : std::false_type {
  };

  template<typename T>
  struct IsOptional<std::optional<T>> final : std::true_type {
  };

  template<typename T>
  inline bool constexpr IsOptionalV = IsOptional<T>::value;
}

#endif // SI_TRAITS_HXX
