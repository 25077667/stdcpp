#include <ranges.hpp>
#include <gtest/gtest.h>

#include <vector>

TEST(stdcpp_ranges, begin)
{
    std::vector<int> v{1, 2, 3};
    auto b = stdcpp::ranges::begin(v);
    ASSERT_EQ(*b, 1);
}

TEST(stdcpp_ranges, end)
{
    std::vector<int> v{1, 2, 3};
    auto e = stdcpp::ranges::end(v);
    ASSERT_EQ(*std::prev(e), 3);
}

TEST(stdcpp_ranges, cbegin)
{
    std::vector<int> v{1, 2, 3};
    auto b = stdcpp::ranges::cbegin(v);
    ASSERT_EQ(*b, 1);
}

TEST(stdcpp_ranges, cend)
{
    std::vector<int> v{1, 2, 3};
    auto e = stdcpp::ranges::cend(v);
    ASSERT_EQ(*std::prev(e), 3);
}

TEST(stdcpp_ranges, rbegin)
{
    std::vector<int> v{1, 2, 3};
    auto b = stdcpp::ranges::rbegin(v);
    ASSERT_EQ(*b, 3);
}

TEST(stdcpp_ranges, rend)
{
    std::vector<int> v{1, 2, 3};
    auto e = stdcpp::ranges::rend(v);
    ASSERT_EQ(*std::prev(e), 1);
}

TEST(stdcpp_ranges, size)
{
    std::vector<int> v{1, 2, 3};
    auto s = stdcpp::ranges::size(v);
    ASSERT_EQ(s, 3u);
}

TEST(stdcpp_ranges, ssize)
{
    std::vector<int> v{1, 2, 3};
    auto s = stdcpp::ranges::ssize(v);
    ASSERT_EQ(s, 3);
}

TEST(stdcpp_ranges, empty)
{
    std::vector<int> v{1, 2, 3};
    auto e = stdcpp::ranges::empty(v);
    ASSERT_FALSE(e);
}

TEST(stdcpp_ranges, data)
{
    std::vector<int> v{1, 2, 3};
    auto d = stdcpp::ranges::data(v);
    ASSERT_EQ(*d, 1);
}

TEST(stdcpp_ranges, cdata)
{
    std::vector<int> v{1, 2, 3};
    auto d = stdcpp::ranges::cdata(v);
    ASSERT_EQ(*d, 1);
}
