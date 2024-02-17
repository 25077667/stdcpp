#include <gtest/gtest.h>
#include <string_view.hpp>

#include <chrono>
#include <unordered_set>

TEST(BasicStringViewTest, DefaultConstructor) {
  stdcpp::string_view sv;
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.size(), 0u);
}

TEST(BasicStringViewTest, CStrConstructor) {
  const char* testStr = "Hello";
  stdcpp::string_view sv(testStr);
  EXPECT_EQ(sv.size(), 5u);
  EXPECT_STREQ(sv.data(), testStr);
}

TEST(BasicStringViewTest, CopyConstructor) {
  stdcpp::string_view original("Hello");
  stdcpp::string_view copy = original;
  EXPECT_EQ(copy, original);
}

TEST(BasicStringViewTest, MoveAssignment) {
  stdcpp::string_view original("Hello");
  stdcpp::string_view moved = std::move(original);
  EXPECT_EQ(moved.size(), 5u);
}

TEST(BasicStringViewTest, At) {
  stdcpp::string_view sv("Hello");
  EXPECT_EQ(sv.at(1), 'e');
  EXPECT_THROW(sv.at(5), std::out_of_range);
}

TEST(BasicStringViewTest, FrontBack) {
  stdcpp::string_view sv("Hello");
  EXPECT_EQ(sv.front(), 'H');
  EXPECT_EQ(sv.back(), 'o');
}

TEST(BasicStringViewTest, OutOfRange) {
  stdcpp::string_view sv("Hello");
  EXPECT_THROW(sv.at(10), std::out_of_range);
}

TEST(BasicStringViewTest, IterationPerformance) {
  const size_t largeSize = 1000000;  // 1 million elements
  std::string largeStr(largeSize, 'a');
  stdcpp::string_view sv(largeStr.c_str(), largeSize);

  auto start = std::chrono::high_resolution_clock::now();
  for (char c : sv) {
    // Simulate minimal processing to avoid loop optimization removal
    asm volatile("" : "+g"(c) : :);
  }
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> elapsed = end - start;
  EXPECT_LT(elapsed.count(), 50) << "Iteration took too long.";
}

TEST(BasicStringViewTest, Iterators) {
  stdcpp::string_view sv("Hello");
  std::string result;

  for (auto it = sv.begin(); it != sv.end(); ++it) {
    result += *it;
  }

  EXPECT_EQ(result, "Hello");

  result.clear();
  for (auto it = sv.rbegin(); it != sv.rend(); ++it) {
    result += *it;
  }

  EXPECT_EQ(result, "olleH");
}

TEST(BasicStringViewTest, Capacity) {
  stdcpp::string_view sv("Test");
  EXPECT_EQ(sv.size(), 4u);
  EXPECT_EQ(sv.length(), 4u);
  EXPECT_FALSE(sv.empty());
}

TEST(BasicStringViewTest, ElementAccess) {
  stdcpp::string_view sv("Test");
  EXPECT_EQ(sv[0], 'T');
  EXPECT_EQ(sv.at(3), 't');
  EXPECT_EQ(sv.front(), 'T');
  EXPECT_EQ(sv.back(), 't');
  EXPECT_THROW(sv.at(4), std::out_of_range);  // Out-of-range access
}

TEST(BasicStringViewTest, Modifiers) {
  stdcpp::string_view sv("Hello, world!");
  sv.remove_prefix(7);
  EXPECT_EQ(sv, "world!");

  sv.remove_suffix(6);
  EXPECT_EQ(sv, "");  // all is removed

  stdcpp::string_view sv2("Test");
  sv.swap(sv2);
  EXPECT_EQ(sv, "Test");
  EXPECT_EQ(sv2, "");
}

TEST(BasicStringViewTest, StringOperations) {
  stdcpp::string_view sv("Hello, world!");
  char buffer[6] = {0};
  sv.copy(buffer, 5);
  EXPECT_EQ(std::string(buffer), "Hello");

  auto sub = sv.substr(7, 5);
  EXPECT_EQ(sub, "world");

  EXPECT_EQ(sv.compare("Hello, world!"), 0);
  EXPECT_TRUE(sv.starts_with("Hello"));
  EXPECT_TRUE(sv.ends_with("world!"));
  EXPECT_TRUE(sv.contains("world"));
}

TEST(BasicStringViewTest, FindOperations) {
  stdcpp::string_view sv("Hello, world!");
  EXPECT_NE(sv.find("world"), stdcpp::string_view::npos);
  EXPECT_EQ(sv.rfind('o'), 8u);
  EXPECT_EQ(sv.find_first_of("aeiou"), 1u);
  EXPECT_EQ(sv.find_last_of("aeiou"), 8u);  // 'o' is the last vowel
  EXPECT_EQ(sv.find_first_not_of("H"), 1u);
  EXPECT_EQ(sv.find_last_not_of("!"), 11u);
}

TEST(BasicStringViewTest, StringViewTypeAliases) {
  stdcpp::string_view sv("Hello");
  stdcpp::wstring_view wsv(L"Hello");
  stdcpp::u16string_view u16sv(u"Hello");
  stdcpp::u32string_view u32sv(U"Hello");

  EXPECT_EQ(sv, "Hello");
  EXPECT_EQ(wsv, L"Hello");
  EXPECT_EQ(u16sv, u"Hello");
  EXPECT_EQ(u32sv, U"Hello");
}

// Helper template to test hashing consistency and uniqueness
template <typename SV, typename S = std::basic_string<typename SV::value_type>>
void test_hash_consistency_and_uniqueness(const S& s1, const S& s2) {
  std::hash<SV> hasher;
  SV sv1(s1), sv2(s2), sv1_copy(s1);

  // Consistency: same content produces the same hash
  EXPECT_EQ(hasher(sv1), hasher(sv1_copy));

  // Uniqueness: different content produces different hashes (most of the time; collisions are possible but should be rare)
  if (s1 != s2) {
    EXPECT_NE(hasher(sv1), hasher(sv2));
  }
}

// Specific tests for empty strings to ensure they don't cause issues
template <typename SV>
void test_empty_string_hash() {
  std::hash<SV> hasher;
  SV empty_sv;

  // Just ensure calling the hash function doesn't throw and returns some value
  EXPECT_NO_THROW(hasher(empty_sv));
}

TEST(HashSpecializationTest, CharHashConsistencyAndUniqueness) {
  test_hash_consistency_and_uniqueness<stdcpp::string_view>("Hello", "World");
}

TEST(HashSpecializationTest, WCharHashConsistencyAndUniqueness) {
  test_hash_consistency_and_uniqueness<stdcpp::wstring_view>(L"Hello",
                                                             L"World");
}

TEST(HashSpecializationTest, Char16THashConsistencyAndUniqueness) {
  test_hash_consistency_and_uniqueness<stdcpp::u16string_view>(u"Hello",
                                                               u"World");
}

TEST(HashSpecializationTest, Char32THashConsistencyAndUniqueness) {
  test_hash_consistency_and_uniqueness<stdcpp::u32string_view>(U"Hello",
                                                               U"World");
}

TEST(HashSpecializationTest, EmptyStringHash) {
  test_empty_string_hash<stdcpp::string_view>();
  test_empty_string_hash<stdcpp::wstring_view>();
  test_empty_string_hash<stdcpp::u16string_view>();
  test_empty_string_hash<stdcpp::u32string_view>();
}
