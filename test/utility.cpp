#include <utility.hpp>
#include <gtest/gtest.h>

enum class EnumType
{
    A,
    B,
    C
};

TEST(Utility, to_underlying_convertEnumToUnderlyingType)
{
    EXPECT_EQ(stdcpp::to_underlying(EnumType::A), 0);
    EXPECT_EQ(stdcpp::to_underlying(EnumType::B), 1);
    EXPECT_EQ(stdcpp::to_underlying(EnumType::C), 2);
}

TEST(Utility, to_underlying_convertEnumToUnderlyingTypeAtCompileTime)
{
    constexpr auto a = stdcpp::to_underlying(EnumType::A);
    constexpr auto b = stdcpp::to_underlying(EnumType::B);
    constexpr auto c = stdcpp::to_underlying(EnumType::C);
    EXPECT_EQ(a, 0);
    EXPECT_EQ(b, 1);
    EXPECT_EQ(c, 2);
}
