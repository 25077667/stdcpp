#ifndef __SCC_STDCPP_TYPE_TRAITS_HPP__
#define __SCC_STDCPP_TYPE_TRAITS_HPP__
#pragma once

#include <type_traits>

namespace stdcpp {
namespace v1 {

template <class T, class U>
constexpr bool is_same_v = std::is_same<T, U>::value;
}

using v1::is_same_v;

}  // namespace stdcpp

#endif  // __SCC_STDCPP_TYPE_TRAITS_HPP__