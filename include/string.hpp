#ifndef __SCC_STDCPP_STRING_HPP__
#define __SCC_STDCPP_STRING_HPP__
#pragma once

#include <string>
#include <cstdint>
namespace stdcpp
{
    using c8_t = std::conditional_t<sizeof(char) == 1, char, std::uint8_t>;
    using u8string = std::basic_string<c8_t>;

    u8string operator"" _u8(const char *str, std::size_t len)
    {
        return u8string(str, len);
    }

    std::string to_string(const u8string &str)
    {
        return std::string(str.begin(), str.end());
    }

    std::string operator+(const std::string &lhs, const u8string &rhs)
    {
        return lhs + to_string(rhs);
    }
}

#endif // __SCC_STDCPP_STRING_HPP__