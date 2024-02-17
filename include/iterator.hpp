#ifndef __SCC_STDCPP_ITERATOR_HPP__
#define __SCC_STDCPP_ITERATOR_HPP__
#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace stdcpp {
namespace v1 {
template <class...>
using void_t = void;

namespace common_reference_detail {
// Helper to combine cv-qualifiers
template <typename T1, typename T2>
struct cv_combiner;

template <typename T>
struct cv_combiner<T, T> {
  using type = T;
};

template <typename T>
struct cv_combiner<const T, T> {
  using type = const T;
};

template <typename T>
struct cv_combiner<T, const T> {
  using type = const T;
};

template <typename T>
struct cv_combiner<volatile T, T> {
  using type = volatile T;
};

template <typename T>
struct cv_combiner<T, volatile T> {
  using type = volatile T;
};

template <typename T>
struct cv_combiner<const volatile T, T> {
  using type = const volatile T;
};

template <typename T>
struct cv_combiner<T, const volatile T> {
  using type = const volatile T;
};

template <typename T>
struct cv_combiner<const T, volatile T> {
  using type = const volatile T;
};

template <typename T>
struct cv_combiner<volatile T, const T> {
  using type = const volatile T;
};

// Primary template for binary_common_reference_t
template <typename T1, typename T2, typename = void>
struct binary_common_reference {};

// Specialization for lvalue references with combinable cv-qualifiers
template <typename X, typename Y>
struct binary_common_reference<
    X&, Y&,
    typename std::enable_if<std::is_reference<
        decltype(false ? std::declval<typename cv_combiner<X, Y>::type&>()
                       : std::declval<typename cv_combiner<Y, X>::type&>())>::
                                value>::type> {
  using type =
      decltype(false ? std::declval<typename cv_combiner<X, Y>::type&>()
                     : std::declval<typename cv_combiner<Y, X>::type&>());
};

// Specialization for rvalue references where a common lvalue reference exists
template <typename T1, typename T2>
struct binary_common_reference<
    T1&&, T2&&,
    void_t<decltype(false
                        ? std::declval<typename cv_combiner<
                              std::decay_t<T1>, std::decay_t<T2>>::type&>()
                        : std::declval<typename cv_combiner<
                              std::decay_t<T2>, std::decay_t<T1>>::type&>())>> {
  using type = typename std::decay<
      decltype(false ? std::declval<typename cv_combiner<
                           std::decay_t<T1>, std::decay_t<T2>>::type&>()
                     : std::declval<typename cv_combiner<
                           std::decay_t<T2>, std::decay_t<T1>>::type&>())>::
      type&&;
};

// Specialization for mixed reference types (one lvalue and one rvalue)
template <typename A, typename B>
struct binary_common_reference<
    A&, B&&,
    void_t<
        decltype(std::declval<typename cv_combiner<A, B>::type&>()),
        std::enable_if_t<std::is_convertible<
            B&&, typename binary_common_reference<
                     A&, typename cv_combiner<A, B>::type&>::type>::value>>> {
  using type =
      typename binary_common_reference<A&,
                                       typename cv_combiner<A, B>::type&>::type;
};

template <typename A, typename B>
struct binary_common_reference<
    B&&, A&,
    void_t<
        decltype(std::declval<typename cv_combiner<B, A>::type&>()),
        std::enable_if_t<std::is_convertible<
            B&&, typename binary_common_reference<
                     A&, typename cv_combiner<B, A>::type&>::type>::value>>> {
  using type =
      typename binary_common_reference<A&,
                                       typename cv_combiner<B, A>::type&>::type;
};

template <typename T, typename U>
using binary_common_reference_t = typename binary_common_reference<T, U>::type;

// multiple types traits, using binary type traits
template <typename T, typename... U>
struct multiple_common_reference {
  using type =
      binary_common_reference_t<T,
                                typename multiple_common_reference<U...>::type>;
};

template <typename T, typename... U>
using multiple_common_reference_t =
    typename multiple_common_reference<T, U...>::type;

// Specialize for the case when there's only one type in U...
template <typename T>
struct multiple_common_reference<T> {
  using type = T;
};

template <typename... T>
struct common_reference {
  using type = typename multiple_common_reference<T...>::type;
};

// Specialization for zero types
template <>
struct common_reference<> {};
}  // namespace common_reference_detail

template <typename T, typename = void>
struct iter_difference {
  using type = std::ptrdiff_t;
};

template <typename T>
struct iter_difference<T, void_t<typename T::difference_type>> {
  using type = typename T::difference_type;
};

template <typename T>
struct iter_value {
  using type = typename T::value_type;
};

template <typename T>
struct iter_value<T*> {
  using type = T;
};

template <typename T>
struct iter_reference {
  using type = decltype(*std::declval<T&>());
};

template <typename T>
struct iter_const_lvalue_reference {
  using type = const T&;  // lvalue reference to const T
};

template <typename T>
struct iter_const_rvalue_reference {
  using type = const T&&;  // rvalue reference to const T
};

template <typename T>
struct iter_const_reference {
  using type = typename common_reference_detail::common_reference<
      const typename iter_value<T>::type&&,
      typename iter_reference<T>::type>::type;
};

template <typename T>
struct iter_rvalue_reference {
  using type = decltype(std::move(*std::declval<T&>()));
};

template <typename T>
struct iter_rvalue_reference<T*> {
  using type = T&&;
};

template <typename... T>
struct common_reference : common_reference_detail::common_reference<T...> {};

template <typename T>
struct iter_common_reference {
  // only self type
  using type = typename common_reference<typename iter_reference<T>::type,
                                         typename iter_value<T>::type&>::type;
};

template <typename T>
using iter_difference_t = typename iter_difference<T>::type;

template <typename T>
using iter_value_t = typename iter_value<T>::type;

template <typename T>
using iter_reference_t = typename iter_reference<T>::type;

template <typename T>
using iter_const_reference_t = typename iter_const_reference<T>::type;

template <typename T>
using iter_rvalue_reference_t = typename iter_rvalue_reference<T>::type;

template <typename... T>
using common_reference_t = typename common_reference<T...>::type;

template <typename T>
using iter_common_reference_t = typename iter_common_reference<T>::type;
}  // namespace v1

using v1::common_reference;
using v1::common_reference_t;
using v1::iter_const_reference;
using v1::iter_const_reference_t;
using v1::iter_difference;
using v1::iter_difference_t;
using v1::iter_reference;
using v1::iter_reference_t;
using v1::iter_rvalue_reference;
using v1::iter_rvalue_reference_t;
using v1::iter_value;
using v1::iter_value_t;
using v1::void_t;
}  // namespace stdcpp

#endif  // __SCC_STDCPP_ITERATOR_HPP__