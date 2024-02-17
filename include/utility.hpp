#ifndef __SCC_STDCPP_UTILITY_HPP__
#define __SCC_STDCPP_UTILITY_HPP__

#include <iterator.hpp>
#include <type_traits.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace stdcpp {
namespace v1 {
template <class Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept {
  return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class T>
constexpr auto ssize(const T& t) noexcept(
    noexcept(static_cast<std::make_signed_t<decltype(t.size())>>(t.size()))) {
  return static_cast<std::make_signed_t<decltype(t.size())>>(t.size());
}

template <class T>
constexpr auto ssize(T&& t) {
  return static_cast<std::make_signed_t<decltype(std::forward<T>(t).size())>>(
      std::forward<T>(t).size());
}

template <class T>
constexpr auto cdata(const T& t) noexcept(noexcept(t.data())) {
  return t.data();
}

template <class T, std::size_t N>
constexpr auto cdata(const T (&t)[N]) noexcept {
  return t;
}

template <class T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept {
  return t;
}

template <class T>
struct add_const {
  using type = const T;
};
}  // namespace v1

template <class Enum>
constexpr auto to_underlying(Enum e) noexcept {
  return v1::to_underlying(e);
}

template <class T>
constexpr auto ssize(const T& t) noexcept(
    noexcept(static_cast<std::make_signed_t<decltype(t.size())>>(t.size()))) {
  return v1::ssize(t);
}

template <class T>
constexpr auto ssize(T&& t) {
  return v1::ssize(std::forward<T>(t));
}

template <class T>
constexpr auto cdata(const T& t) noexcept(noexcept(t.data())) {
  return v1::cdata(t);
}

template <class T, std::size_t N>
constexpr auto cdata(const T (&t)[N]) noexcept {
  return v1::cdata(t);
}

template <class T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept {
  return v1::as_const(t);
}

template <class T>
struct add_const {
  using type = typename v1::add_const<T>::type;
};
}  // namespace stdcpp

#endif  // __SCC_STDCPP_UTILITY_HPP__