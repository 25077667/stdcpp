#include <gtest/gtest.h>
#include <type_traits.hpp>

#include <list>
#include <vector>

TEST(IsSameVTest, SameBasicTypes) {
  EXPECT_TRUE((stdcpp::is_same_v<int, int>));
  EXPECT_TRUE((stdcpp::is_same_v<char, char>));
  EXPECT_TRUE((stdcpp::is_same_v<float, float>));
}

TEST(IsSameVTest, DifferentBasicTypes) {
  EXPECT_FALSE((stdcpp::is_same_v<int, char>));
  EXPECT_FALSE((stdcpp::is_same_v<float, int>));
  EXPECT_FALSE((stdcpp::is_same_v<double, float>));
}

class Base {};
class Derived : public Base {};

TEST(IsSameVTest, ClassTypes) {
  EXPECT_FALSE((stdcpp::is_same_v<Base, Derived>));
  EXPECT_TRUE((stdcpp::is_same_v<Base, Base>));
  EXPECT_TRUE((stdcpp::is_same_v<Derived, Derived>));
}

TEST(IsSameVTest, STLContainerTypes) {
  EXPECT_TRUE((stdcpp::is_same_v<std::vector<int>, std::vector<int>>));
  EXPECT_FALSE((stdcpp::is_same_v<std::vector<int>, std::vector<float>>));
  EXPECT_FALSE((stdcpp::is_same_v<std::vector<int>, std::list<int>>));
}

TEST(IsSameVTest, ConstAndPointerTypes) {
  EXPECT_TRUE((stdcpp::is_same_v<int*, int*>));
  EXPECT_FALSE((stdcpp::is_same_v<int*, const int*>));
  EXPECT_TRUE((stdcpp::is_same_v<const int, const int>));
  EXPECT_FALSE((stdcpp::is_same_v<const int, int>));
}
