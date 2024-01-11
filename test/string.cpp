#include <string.hpp>
#include <gtest/gtest.h>

TEST(stdcpp_string, u8string)
{
    stdcpp::u8string s = u8"Hello, world!";
    EXPECT_EQ(s, u8"Hello, world!");
}

