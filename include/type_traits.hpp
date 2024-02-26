#ifndef __SCC_STDCPP_TYPE_TRAITS_HPP__
#define __SCC_STDCPP_TYPE_TRAITS_HPP__
#pragma once

#include <type_traits>

namespace stdcpp {
namespace v1 {

template <class T, class U>
constexpr bool is_same_v = std::is_same<T, U>::value;

template <class T>
constexpr bool is_class_v = std::is_class<T>::value;
}  // namespace v1

using v1::is_class_v;
using v1::is_same_v;

}  // namespace stdcpp

#endif  // __SCC_STDCPP_TYPE_TRAITS_HPP__