#ifndef __SCC_STDCPP_RANGES_HPP__
#define __SCC_STDCPP_RANGES_HPP__
#pragma once

#include <utility.hpp>

#include <iterator>
#include <type_traits>

namespace stdcpp
{
    namespace v1
    {
        namespace ranges
        {
            namespace views
            {
            }

            template <class T>
            constexpr auto begin(T &&r) -> decltype(auto)
            {
                return std::begin(std::forward<T>(r));
            };

            template <class T>
            constexpr auto end(T &&r) -> decltype(auto)
            {
                return std::end(std::forward<T>(r));
            };

            template <class T>
            constexpr auto cbegin(T &&r) -> decltype(auto)
            {
                return std::cbegin(std::forward<T>(r));
            };

            template <class T>
            constexpr auto cend(T &&r) -> decltype(auto)
            {
                return std::cend(std::forward<T>(r));
            };

            template <class T>
            constexpr auto rbegin(T &&r) -> decltype(auto)
            {
                return std::rbegin(std::forward<T>(r));
            };

            template <class T>
            constexpr auto rend(T &&r) -> decltype(auto)
            {
                return std::rend(std::forward<T>(r));
            };

            template <class T>
            constexpr auto size(T &&r) -> decltype(auto)
            {
                return r.size();
            };

            template <class T, size_t N>
            constexpr auto size(const T (&)[N]) noexcept
            {
                return N;
            }

            template <class T>
            constexpr auto ssize(T &&r) -> decltype(auto)
            {
                return stdcpp::ssize(std::forward<T>(r));
            };

            template <class T>
            constexpr auto empty(T &&r) -> decltype(auto)
            {
                return r.empty();
            };

            template <class T, size_t N>
            constexpr auto empty(const T (&)[N]) noexcept
            {
                return false;
            }

            template <class T>
            constexpr auto empty(std::initializer_list<T> il) noexcept
            {
                return il.size() == 0;
            }

            template <class T>
            constexpr auto data(T &&r) -> decltype(auto)
            {
                return r.data();
            };

            template <class T>
            constexpr auto data(const T &r) -> decltype(auto)
            {
                return r.data();
            };

            template <class T, size_t N>
            constexpr auto data(const T (&a)[N]) noexcept
            {
                return a;
            }

            template <class T>
            constexpr auto data(std::initializer_list<T> il) noexcept
            {
                return il.begin();
            }

            template <class T>
            constexpr auto cdata(T &&r) -> decltype(auto)
            {
                return stdcpp::cdata(std::forward<T>(r));
            };
        }

        // using views = ranges::views;
    }

    using namespace v1;
}

#endif // __SCC_STDCPP_RANGES_HPP__