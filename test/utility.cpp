#include <utility.hpp>
#include <gtest/gtest.h>

#include <vector>
#include <array>
#include <type_traits>

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

TEST(Utility, ssize_convertUnsignedToSigned)
{
    std::vector<int> v{1, 2, 3};
    EXPECT_EQ(stdcpp::ssize(v), 3);
}

TEST(ToUnderlyingTest, ConvertsEnumCorrectly)
{
    enum class Color : unsigned int
    {
        Red = 1,
        Green,
        Blue
    };
    enum class Mood : int
    {
        Happy = 1,
        Sad = -1
    };

    EXPECT_EQ(stdcpp::to_underlying(Color::Red), 1u);
    EXPECT_EQ(stdcpp::to_underlying(Color::Green), 2u);
    EXPECT_EQ(stdcpp::to_underlying(Mood::Happy), 1);
    EXPECT_EQ(stdcpp::to_underlying(Mood::Sad), -1);
}

TEST(ToUnderlyingTest, HandlesMinMaxValues)
{
    enum class Numbers : int
    {
        Min = std::numeric_limits<int>::min(),
        Max = std::numeric_limits<int>::max()
    };
    EXPECT_EQ(stdcpp::to_underlying(Numbers::Min), std::numeric_limits<int>::min());
    EXPECT_EQ(stdcpp::to_underlying(Numbers::Max), std::numeric_limits<int>::max());
}

// Test Suite for ssize
TEST(SSizeTest, ReturnsCorrectSignedSize)
{
    std::vector<int> vec = {1, 2, 3};
    std::string str = "Hello";
    auto vec_size = stdcpp::ssize(vec);
    auto str_size = stdcpp::ssize(str);

    static_assert(std::is_signed<decltype(vec_size)>::value, "ssize should return a signed size for vector");
    static_assert(std::is_signed<decltype(str_size)>::value, "ssize should return a signed size for string");

    EXPECT_EQ(vec_size, 3);
    EXPECT_EQ(str_size, 5);
}

TEST(SSizeTest, WorksForEmptyContainer)
{
    std::vector<int> vec;
    std::string str = "";
    EXPECT_EQ(stdcpp::ssize(vec), 0);
    EXPECT_EQ(stdcpp::ssize(str), 0);
}

TEST(SSizeTest, HandlesLargeContainer)
{
    std::vector<int> largeVec(std::numeric_limits<int>::max() / 2);
    auto size = stdcpp::ssize(largeVec);
    EXPECT_GE(size, 0); // Just checking that it doesn't underflow or throw
}

// Test Suite for cdata
TEST(CDataTest, ReturnsPointerToData)
{
    std::vector<int> vec = {1, 2, 3};
    std::string str = "Hello";
    EXPECT_EQ(stdcpp::cdata(vec), vec.data());
    EXPECT_EQ(stdcpp::cdata(str), str.data());
}

TEST(CDataTest, WorksForEmptyContainer)
{
    std::vector<int> vec;
    std::string str = "";
    EXPECT_EQ(stdcpp::cdata(vec), vec.data());
    EXPECT_EQ(stdcpp::cdata(str), str.data());
}

TEST(CDataTest, WorksForCStyleArray)
{
    int arr[] = {1, 2, 3};
    EXPECT_EQ(stdcpp::cdata(arr), std::begin(arr));
}

TEST(CommonReferenceTest, BasicTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::common_reference_t<int>, int>, "Common reference of int should be int");
    static_assert(stdcpp::is_same_v<stdcpp::common_reference_t<int &>, int &>, "Common reference of int& should be int&");
}

TEST(IteratorTraitsTest, ValueTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_value_t<std::vector<int>::iterator>, int>, "Iterator value type should be int for vector<int>");
}

TEST(IteratorTraitsTest, ReferenceTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_reference_t<std::vector<int>::iterator>, int &>, "Iterator reference type should be int& for vector<int>");
}

TEST(IteratorTraitsTest, RValueReferenceTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_rvalue_reference_t<std::vector<int>::iterator>, int &&>, "Iterator rvalue reference type should be int&& for vector<int>");
}

TEST(IteratorTraitsTest, DifferenceTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_difference_t<std::vector<int>::iterator>, std::ptrdiff_t>, "Iterator difference type should be ptrdiff_t for vector<int>");
}

TEST(IteratorTraitsTest, ConstValueTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_value_t<std::vector<int>::const_iterator>, int>, "Iterator value type should be int for vector<int>");
}

TEST(IteratorTraitsTest, ConstReferenceTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_reference_t<std::vector<int>::const_iterator>, const int &>, "Iterator reference type should be const int& for vector<int>");
}

TEST(IteratorTraitsTest, CommonReferenceTest)
{
    static_assert(stdcpp::is_same_v<stdcpp::iter_common_reference_t<std::vector<int>::iterator>, stdcpp::iter_reference_t<std::vector<int>::iterator>>, "Iterator common reference type should be iterator reference type for vector<int>");
}

TEST(IterMoveTest, BasicTest)
{
    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    auto moved = stdcpp::iter_move(it);
    EXPECT_EQ(moved, 1);
}