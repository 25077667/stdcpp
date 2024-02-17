#include <gtest/gtest.h>
#include <iterator.hpp>

#include <type_traits.hpp>

TEST(CvCombinerTest, BasicCombinations) {
  static_assert(
      stdcpp::is_same_v<
          stdcpp::v1::common_reference_detail::cv_combiner<int, int>::type,
          int>,
      "Same type should not change.");
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::cv_combiner<
                            const int, int>::type,
                        const int>,
      "Combining const and non-const should result in const.");
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::cv_combiner<
                            int, volatile int>::type,
                        volatile int>,
      "Combining volatile and non-volatile should result in volatile.");
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::cv_combiner<
                            const int, volatile int>::type,
                        const volatile int>,
      "Combining const and volatile should result in const volatile.");
}

TEST(BinaryCommonReferenceTest, ReferenceTypes) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::
                            binary_common_reference<int&, int&>::type,
                        int&>,
      "Lvalue references should deduce to lvalue reference of the same type.");
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::
                            binary_common_reference<const int&, int&>::type,
                        const int&>,
      "Lvalue reference with const should deduce to const lvalue reference.");
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::
                            binary_common_reference<int&&, int&&>::type,
                        int&&>,
      "Rvalue references should deduce to rvalue reference of the same type.");
  // The following common reference should not compile
  //   stdcpp::v1::common_reference_detail::binary_common_reference<int&, int&&>::type
  // See the last rule: https://en.cppreference.com/w/cpp/types/common_reference
  //
  // * Otherwise, one of the two types must be an lvalue reference type A& and the other must
  // * be an rvalue reference type B&& (A and B might be cv-qualified). Let D denote the simple
  // * common reference type of A& and B const&, if any. If D exists and std::is_convertible_v<B&&, D>
  // * is true, then the simple common reference type is D;
  //
  // (A and B might be cv-qualified)
  // However, <int&, int&&> are not cv-qualified, so the rule does not apply.
  // https://godbolt.org/z/81n6jP87j

  // So, here is the test for the rule:
  static_assert(
      stdcpp::is_same_v<
          stdcpp::v1::common_reference_detail::binary_common_reference<
              const int&, const int&&>::type,
          const int&>,
      "Lvalue reference with const should deduce to rvalue reference.");
}

// CustomIterator type
struct CustomIterator {
  using difference_type = std::ptrdiff_t;
  using value_type = int;
  using pointer = int*;
  using reference = int&;
  using iterator_category = std::random_access_iterator_tag;
  //   operator dereference() const;
  value_type& operator*() const {
    // Assuming there's an internal mechanism to obtain the current value
    static value_type tmp = 10;  // Example implementation detail
    return tmp;
  }
};

using CustomDifferenceType = std::ptrdiff_t;
using CustomValueType = int;
using CustomReferenceType = int&;
using CommonReferenceType = int&;

TEST(IteratorTraitsTest, IterDifference) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_difference_t<int*>, std::ptrdiff_t>,
      "Pointer difference type should be ptrdiff_t.");
  // Assuming a custom iterator type is defined elsewhere with a difference_type
  static_assert(stdcpp::is_same_v<stdcpp::v1::iter_difference_t<CustomIterator>,
                                  CustomDifferenceType>,
                "Custom iterator should use its own difference_type.");
}

TEST(IteratorTraitsTest, IterValue) {
  static_assert(stdcpp::is_same_v<stdcpp::v1::iter_value_t<int*>, int>,
                "Pointer value type should deduce to pointed type.");
  // Assuming a custom iterator type is defined elsewhere with a value_type
  static_assert(stdcpp::is_same_v<stdcpp::v1::iter_value_t<CustomIterator>,
                                  CustomValueType>,
                "Custom iterator should use its own value_type.");
}

TEST(IteratorTraitsTest, IterReference) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_reference_t<int*>, int&>,
      "Pointer reference should deduce to reference of pointed type.");
  // Custom iterator test similar to iter_value_t
}

TEST(IteratorTraitsTest, IterConstReference) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_const_reference_t<int*>, const int&>,
      "Pointer const reference should deduce to const reference of pointed "
      "type.");
  // Custom iterator test similar to iter_const_reference_t
}

TEST(IteratorTraitsTest, IterRvalueReference) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_rvalue_reference_t<int*>, int&&>,
      "Pointer rvalue reference should deduce to rvalue reference of pointed "
      "type.");
  // Custom iterator test similar to iter_rvalue_reference_t
}

TEST(IteratorTraitsTest, IterCommonReference) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_common_reference_t<int*>, int&>,
      "Pointer common reference should deduce according to common "
      "reference rules.");
  // Custom iterator test similar to iter_common_reference_t
}

TEST(CvCombinerTest, ComplexCombinations) {
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::cv_combiner<
                            volatile int, const int>::type,
                        const volatile int>,
      "Combining const and volatile should result in const volatile.");
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::common_reference_detail::cv_combiner<
                            const volatile int, int>::type,
                        const volatile int>,
      "Combining int with const volatile should result in const volatile.");
}

TEST(MultipleCommonReferenceTest, EdgeCases) {
  static_assert(
      stdcpp::is_same_v<
          stdcpp::v1::common_reference_t<int&, const int&, volatile int&>,
          const volatile int&>,
      "Should handle mixed cv-qualifiers across multiple types.");
}

TEST(CommonReferenceTest, EdgeCasesAndSingleType) {
  static_assert(stdcpp::is_same_v<stdcpp::v1::common_reference_t<int>, int>,
                "Single type should deduce to itself.");
}

TEST(IteratorTraitsTest, CustomIteratorDifferenceType) {
  // Assuming CustomIterator defines a custom difference_type
  static_assert(stdcpp::is_same_v<stdcpp::v1::iter_difference_t<CustomIterator>,
                                  CustomDifferenceType>,
                "Custom iterator should use its specified difference_type.");
}

TEST(IteratorTraitsTest, CustomIteratorValueType) {
  // Assuming CustomIterator defines a custom value_type
  static_assert(stdcpp::is_same_v<stdcpp::v1::iter_value_t<CustomIterator>,
                                  CustomValueType>,
                "Custom iterator should use its specified value_type.");
}

TEST(IteratorTraitsTest, CustomIteratorReferenceType) {
  // Assuming CustomIterator defines a specific way to reference its value
  static_assert(stdcpp::is_same_v<stdcpp::v1::iter_reference_t<CustomIterator>,
                                  CustomReferenceType>,
                "Custom iterator should deduce its reference type correctly.");
}

TEST(IteratorTraitsTest, CustomIteratorRvalueReference) {
  // Assuming CustomIterator allows moving its value
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_rvalue_reference_t<CustomIterator>,
                        CustomValueType&&>,
      "Custom iterator should deduce its rvalue reference type correctly.");
}

TEST(IteratorTraitsTest, CustomIteratorCommonReference) {
  // Assuming CustomIterator and another type for common reference testing
  static_assert(
      stdcpp::is_same_v<stdcpp::v1::iter_common_reference_t<CustomIterator>,
                        CommonReferenceType>,
      "Custom iterator should deduce its common reference type correctly with "
      "other types.");
}
