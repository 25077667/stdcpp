#ifndef __SCC_STDCPP_UTILITY_HPP__
#define __SCC_STDCPP_UTILITY_HPP__

#include <type_traits>
#include <utility>

namespace stdcpp
{
    namespace v1
    {
        template <class Enum>
        constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
        {
            return static_cast<std::underlying_type_t<Enum>>(e);
        }

        template <class Enum>
        struct __ToUnderlying
        {
            static constexpr auto value = v1::to_underlying<Enum>;
        };
    }

    template <class Enum>
    constexpr auto to_underlying(Enum e) noexcept
    {
        return v1::__ToUnderlying<Enum>::value(e);
    }

    template <class T>
    constexpr auto ssize(const T &t) noexcept(noexcept(static_cast<std::make_signed_t<decltype(t.size())>>(t.size())))
    {
        return static_cast<std::make_signed_t<decltype(t.size())>>(t.size());
    }

    template <class T>
    constexpr auto ssize(T &&t)
    {
        return static_cast<std::make_signed_t<decltype(std::forward<T>(t).size())>>(std::forward<T>(t).size());
    }

    template <class T>
    constexpr auto cdata(const T &t) noexcept(noexcept(t.data()))
    {
        return t.data();
    }
}

#endif // __SCC_STDCPP_UTILITY_HPP__