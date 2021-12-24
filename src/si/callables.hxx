#pragma once

#ifndef SI_CALLABLES_HXX
#define SI_CALLABLES_HXX

#include <concepts>

namespace si {
  namespace detail {
    template<std::size_t N, typename... Types>
    struct nth;

    template<typename Head, typename... Tail>
    struct nth<0u, Head, Tail...> {
      using type = Head;
    };

    template<std::size_t N, typename Head, typename... Tail>
    struct nth<N, Head, Tail...> : nth<N-1, Tail...> {
    };

    template<std::size_t N, typename... Types>
    using nth_t = typename nth<N, Types...>::type;

    template<typename Callable>
    struct CallableTraits : CallableTraits<decltype(&Callable::operator())> {
    };

    template<typename Return, typename... Args>
    struct CallableTraits<Return(Args...)> {
      using return_type = Return;
      static constexpr auto argument_count = sizeof...(Args);
      template<std::size_t N>
      using argument_type = nth_t<N, Args...>;
    };

    template<typename Return, typename... Args>
    struct CallableTraits<Return(*)(Args...)> {
      using return_type = Return;
      static constexpr auto argument_count = sizeof...(Args);
      template<std::size_t N>
      using argument_type = nth_t<N, Args...>;
    };

    template<typename Return, typename... Args>
    struct CallableTraits<Return(&)(Args...)> {
      using return_type = Return;
      static constexpr auto argument_count = sizeof...(Args);
      template<std::size_t N>
      using argument_type = nth_t<N, Args...>;
    };

    template<typename Return, typename Class, typename... Args>
    struct CallableTraits<Return(Class::*)(Args...)> {
      using return_type = Return;
      static constexpr auto argument_count = sizeof...(Args);
      template<std::size_t N>
      using argument_type = nth_t<N, Args...>;
    };

    template<typename Return, typename Class, typename... Args>
    struct CallableTraits<Return(Class::*)(Args...) const> {
      using return_type = Return;
      static constexpr auto argument_count = sizeof...(Args);
      template<std::size_t N>
      using argument_type = nth_t<N, Args...>;
    };
  }

  template<typename T>
  concept NotVoid = !std::is_same_v<T, void>;

  template<typename T>
  concept ReturnsValue = !std::is_same_v<typename detail::CallableTraits<T>::return_type, void>;

  template<typename T, typename Value>
  concept ReturnsValueOf = std::is_same_v<typename detail::CallableTraits<T>::return_type, Value>;

  template<typename T>
  concept TakesNoArgument = detail::CallableTraits<T>::argument_count==0;

  template<typename T>
  concept TakesSingleArgument = detail::CallableTraits<T>::argument_count==1;

  template<typename T>
  concept TakesTwoArguments = detail::CallableTraits<T>::argument_count==2;

  template<typename T>
  concept ReturnsFirstArgumentType = std::is_same_v<
      typename detail::CallableTraits<T>::template argument_type<0u>,
      typename detail::CallableTraits<T>::return_type
  >;

  template<typename T>
  concept Consumer = TakesSingleArgument<T>;

  template<typename T>
  concept BinaryConsumer = TakesTwoArguments<T>;

  template<typename T>
  concept Producer = TakesNoArgument<T> && ReturnsValue<T>;

  template<typename T>
  concept Predicate = TakesSingleArgument<T> && ReturnsValueOf<T, bool>;

  template<typename T>
  concept BinaryPredicate = TakesTwoArguments<T> && ReturnsValueOf<T, bool>;

  template<typename T>
  concept Mapper = TakesSingleArgument<T> && ReturnsValue<T>;

  template<typename T>
  concept BinaryMapper = TakesTwoArguments<T> && ReturnsValue<T>;

  template<typename T>
  concept Aggregator = TakesTwoArguments<T> && ReturnsFirstArgumentType<T>;
}

#endif // SI_CALLABLES_HXX
