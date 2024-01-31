#include <string.hpp>
#include <gtest/gtest.h>

TEST(U8StringTest, LiteralOperatorCorrectness)
{
    auto result = u8"hello";
    stdcpp::u8string expected("hello");
    EXPECT_EQ(result, expected);
}

TEST(U8StringTest, LiteralOperatorWithEmptyString)
{
    auto result = u8"";
    stdcpp::u8string expected("");
    EXPECT_EQ(result, expected);
}

// Test Suite for to_string Function
TEST(ToStringTest, ConvertsU8StringToStdString)
{
    stdcpp::u8string input("hello");
    std::string result = stdcpp::to_string(input);
    std::string expected("hello");
    EXPECT_EQ(result, expected);
}

TEST(ToStringTest, ConvertsEmptyU8String)
{
    stdcpp::u8string input("");
    std::string result = stdcpp::to_string(input);
    std::string expected("");
    EXPECT_EQ(result, expected);
}

// Test Suite for Operator+ (String Concatenation)
TEST(OperatorPlusTest, ConcatenatesStdStringAndU8String)
{
    std::string lhs("Hello, ");
    stdcpp::u8string rhs(u8"world!");
    std::string result = lhs + rhs;
    std::string expected("Hello, world!");
    EXPECT_EQ(result, expected);
}

TEST(OperatorPlusTest, ConcatenatesWithEmptyStdString)
{
    std::string lhs("");
    stdcpp::u8string rhs(u8"world!");
    std::string result = lhs + rhs;
    std::string expected("world!");
    EXPECT_EQ(result, expected);
}

TEST(OperatorPlusTest, ConcatenatesWithEmptyU8String)
{
    std::string lhs("Hello, ");
    stdcpp::u8string rhs(u8"");
    std::string result = lhs + rhs;
    std::string expected("Hello, ");
    EXPECT_EQ(result, expected);
}
