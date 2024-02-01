#include <gtest/gtest.h>
#include <ranges.hpp>

#include <array>
#include <list>
#include <string>
#include <vector>

TEST(stdcpp_ranges, begin) {
  std::vector<int> v{1, 2, 3};
  auto b = stdcpp::ranges::begin(v);
  ASSERT_EQ(*b, 1);
}

TEST(stdcpp_ranges, end) {
  std::vector<int> v{1, 2, 3};
  auto e = stdcpp::ranges::end(v);
  ASSERT_EQ(*std::prev(e), 3);
}

TEST(stdcpp_ranges, cbegin) {
  std::vector<int> v{1, 2, 3};
  auto b = stdcpp::ranges::cbegin(v);
  ASSERT_EQ(*b, 1);
}

TEST(stdcpp_ranges, cend) {
  std::vector<int> v{1, 2, 3};
  auto e = stdcpp::ranges::cend(v);
  ASSERT_EQ(*std::prev(e), 3);
}

TEST(stdcpp_ranges, rbegin) {
  std::vector<int> v{1, 2, 3};
  auto b = stdcpp::ranges::rbegin(v);
  ASSERT_EQ(*b, 3);
}

TEST(stdcpp_ranges, rend) {
  std::vector<int> v{1, 2, 3};
  auto e = stdcpp::ranges::rend(v);
  ASSERT_EQ(*std::prev(e), 1);
}

TEST(stdcpp_ranges, size) {
  std::vector<int> v{1, 2, 3};
  auto s = stdcpp::ranges::size(v);
  ASSERT_EQ(s, 3u);
}

TEST(stdcpp_ranges, ssize) {
  std::vector<int> v{1, 2, 3};
  auto s = stdcpp::ranges::ssize(v);
  ASSERT_EQ(s, 3);
}

TEST(stdcpp_ranges, empty) {
  std::vector<int> v{1, 2, 3};
  auto e = stdcpp::ranges::empty(v);
  ASSERT_FALSE(e);
}

TEST(stdcpp_ranges, data) {
  std::vector<int> v{1, 2, 3};
  auto d = stdcpp::ranges::data(v);
  ASSERT_EQ(*d, 1);
}

TEST(stdcpp_ranges, cdata) {
  std::vector<int> v{1, 2, 3};
  auto d = stdcpp::ranges::cdata(v);
  ASSERT_EQ(*d, 1);
}

TEST(stdcpp_ranges, IteratorT) {
  static_assert(stdcpp::is_same_v<stdcpp::ranges::iterator_t<std::vector<int>>,
                                  std::vector<int>::iterator>,
                "Iterator type should be std::vector<int>::iterator for "
                "std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::iterator_t<const std::vector<int>>,
                        std::vector<int>::const_iterator>,
      "Iterator type should be std::vector<int>::const_iterator for const "
      "std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::iterator_t<std::list<std::string>>,
                        std::list<std::string>::iterator>,
      "Iterator type should be std::list<std::string>::iterator for "
      "std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::iterator_t<std::array<char, 5>>,
                        std::array<char, 5>::iterator>,
      "Iterator type should be std::array<char, 5>::iterator for "
      "std::array<char, 5>");
}

TEST(stdcpp_ranges, SentinelT) {
  static_assert(stdcpp::is_same_v<stdcpp::ranges::sentinel_t<std::vector<int>>,
                                  std::vector<int>::iterator>,
                "Sentinel type should be std::vector<int>::iterator for "
                "std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::sentinel_t<const std::vector<int>>,
                        std::vector<int>::const_iterator>,
      "Sentinel type should be std::vector<int>::const_iterator for const "
      "std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::sentinel_t<std::list<std::string>>,
                        std::list<std::string>::iterator>,
      "Sentinel type should be std::list<std::string>::iterator for "
      "std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::sentinel_t<std::array<char, 5>>,
                        std::array<char, 5>::iterator>,
      "Sentinel type should be std::array<char, 5>::iterator for "
      "std::array<char, 5>");
}

TEST(stdcpp_ranges, const_iterator_t) {
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::const_iterator_t<std::vector<int>>,
                        std::vector<int>::const_iterator>,
      "Const iterator type should be std::vector<int>::const_iterator for "
      "std::vector<int>");
  static_assert(stdcpp::is_same_v<
                    stdcpp::ranges::const_iterator_t<const std::vector<int>>,
                    std::vector<int>::const_iterator>,
                "Const iterator type should be "
                "std::vector<int>::const_iterator for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::const_iterator_t<std::list<std::string>>,
          std::list<std::string>::const_iterator>,
      "Const iterator type should be std::list<std::string>::const_iterator "
      "for std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::const_iterator_t<std::array<char, 5>>,
                        std::array<char, 5>::const_iterator>,
      "Const iterator type should be std::array<char, 5>::const_iterator for "
      "std::array<char, 5>");
}

TEST(stdcpp_ranges, const_sentinel_t) {
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::const_sentinel_t<std::vector<int>>,
                        std::vector<int>::const_iterator>,
      "Const sentinel type should be std::vector<int>::const_iterator for "
      "std::vector<int>");
  static_assert(stdcpp::is_same_v<
                    stdcpp::ranges::const_sentinel_t<const std::vector<int>>,
                    std::vector<int>::const_iterator>,
                "Const sentinel type should be "
                "std::vector<int>::const_iterator for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::const_sentinel_t<std::list<std::string>>,
          std::list<std::string>::const_iterator>,
      "Const sentinel type should be std::list<std::string>::const_iterator "
      "for std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::const_sentinel_t<std::array<char, 5>>,
                        std::array<char, 5>::const_iterator>,
      "Const sentinel type should be std::array<char, 5>::const_iterator for "
      "std::array<char, 5>");
}

TEST(stdcpp_ranges, range_difference_t) {
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_difference_t<std::vector<int>>,
                        std::ptrdiff_t>,
      "Difference type should be std::ptrdiff_t for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_difference_t<const std::vector<int>>,
          std::ptrdiff_t>,
      "Difference type should be std::ptrdiff_t for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_difference_t<std::list<std::string>>,
          std::ptrdiff_t>,
      "Difference type should be std::ptrdiff_t for std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_difference_t<std::array<char, 5>>,
                        std::ptrdiff_t>,
      "Difference type should be std::ptrdiff_t for std::array<char, 5>");
}

TEST(stdcpp_ranges, range_size_t) {
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_size_t<std::vector<int>>,
                        std::size_t>,
      "Size type should be std::size_t for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_size_t<const std::vector<int>>,
                        std::size_t>,
      "Size type should be std::size_t for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_size_t<std::list<std::string>>,
                        std::size_t>,
      "Size type should be std::size_t for std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_size_t<std::array<char, 5>>,
                        std::size_t>,
      "Size type should be std::size_t for std::array<char, 5>");
}

TEST(stdcpp_ranges, range_value_t) {
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_value_t<std::vector<int>>, int>,
      "Value type should be int for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_value_t<const std::vector<int>>,
                        int>,
      "Value type should be int for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_value_t<std::list<std::string>>,
                        std::string>,
      "Value type should be std::string for std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_value_t<std::array<char, 5>>,
                        char>,
      "Value type should be char for std::array<char, 5>");
}

TEST(stdcpp_ranges, range_reference_t) {
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_reference_t<std::vector<int>>,
                        int&>,
      "Reference type should be int& for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_reference_t<const std::vector<int>>,
          const int&>,
      "Reference type should be const int& for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_reference_t<std::list<std::string>>,
          std::string&>,
      "Reference type should be std::string& for std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<stdcpp::ranges::range_reference_t<std::array<char, 5>>,
                        char&>,
      "Reference type should be char& for std::array<char, 5>");
}

TEST(stdcpp_ranges, range_const_reference_t) {
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_const_reference_t<std::vector<int>>,
          const int&>,
      "Const reference type should be const int& for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_const_reference_t<const std::vector<int>>,
          const int&>,
      "Const reference type should be const int& for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_const_reference_t<std::list<std::string>>,
          const std::string&>,
      "Const reference type should be const std::string& for "
      "std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_const_reference_t<std::array<char, 5>>,
          const char&>,
      "Const reference type should be const char& for std::array<char, 5>");
}

TEST(stdcpp_ranges, range_rvalue_reference_t) {
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_rvalue_reference_t<std::vector<int>>, int&&>,
      "Rvalue reference type should be int&& for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_rvalue_reference_t<const std::vector<int>>,
          const int&&>,
      "Rvalue reference type should be const int&& for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_rvalue_reference_t<std::list<std::string>>,
          std::string&&>,
      "Rvalue reference type should be std::string&& for "
      "std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_rvalue_reference_t<std::array<char, 5>>,
          char&&>,
      "Rvalue reference type should be char&& for std::array<char, 5>");
}

TEST(stdcpp_ranges, range_common_reference_t) {
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_common_reference_t<std::vector<int>>, int&>,
      "Common reference type should be int& for std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_common_reference_t<const std::vector<int>>,
          const int&>,
      "Common reference type should be const int& for const std::vector<int>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_common_reference_t<std::list<std::string>>,
          std::string&>,
      "Common reference type should be std::string& for "
      "std::list<std::string>");
  static_assert(
      stdcpp::is_same_v<
          stdcpp::ranges::range_common_reference_t<std::array<char, 5>>, char&>,
      "Common reference type should be char& for std::array<char, 5>");
}