#ifndef __SCC_STDCPP_UTILITY_HPP__
#define __SCC_STDCPP_UTILITY_HPP__

#include <type_traits>

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
}

#endif // __SCC_STDCPP_UTILITY_HPP__