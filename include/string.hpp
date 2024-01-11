#ifndef __SCC_STDCPP_STRING_HPP__
#define __SCC_STDCPP_STRING_HPP__
#pragma once

// #include <algorithm.hpp>

#include <string>
#include <cstdint>
namespace stdcpp
{
    // if char8_t is not defined, define it
    // if char8_t is defined, do nothing
    using c8_t = std::conditional_t<sizeof(char) == 1, char, std::uint8_t>;
    using u8string = std::basic_string<c8_t>;

    u8string operator"" _u8(const char *str, std::size_t len)
    {
        return u8string(str, len);
    }
}

#endif // __SCC_STDCPP_STRING_HPP__