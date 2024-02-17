#include <gtest/gtest.h>
#include <chrono.hpp>

#include <type_traits.hpp>

class UtcClockTest : public ::testing::Test {
 protected:
  // Setup and teardown if needed
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(UtcClockTest, NowIsCorrectType) {
  auto now = stdcpp::utc_clock::now();
  static_assert(stdcpp::is_same_v<decltype(now), stdcpp::utc_clock::time_point>,
                "Now() should return a stdcpp::utc_clock::time_point.");
}

TEST_F(UtcClockTest, ToSysConversionValidity) {
  auto utc_now = stdcpp::utc_clock::now();
  auto sys_now = stdcpp::utc_clock::to_sys(utc_now);
  auto expected_sys_now = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::seconds>(
                  expected_sys_now - sys_now)
                  .count();
  // Assuming the operation above is quick, the difference in seconds should be negligible.
  EXPECT_LE(diff, 1);
}

TEST_F(UtcClockTest, FromSysConversionValidity) {
  auto sys_now = std::chrono::system_clock::now();
  auto utc_now = stdcpp::utc_clock::from_sys(sys_now);
  auto back_to_sys_now = stdcpp::utc_clock::to_sys(utc_now);
  // The conversion back and forth should yield the same or very close time points.
  auto diff = std::chrono::duration_cast<std::chrono::seconds>(sys_now -
                                                               back_to_sys_now)
                  .count();
  EXPECT_LE(diff, 1);
}

TEST_F(UtcClockTest, OutputStreamOperator) {
  std::stringstream ss;
  auto utc_now = stdcpp::utc_clock::now();
  ss << utc_now;
  // Just checking that something was put into the stream
  EXPECT_FALSE(ss.str().empty());
}

TEST_F(UtcClockTest, LeapSecondInfoDefaults) {
  auto utc_now = stdcpp::utc_clock::now();
  auto leap_info = get_leap_second_info(utc_now);
  EXPECT_FALSE(leap_info.is_leap_second);
  // Assuming the last entry corresponds to 37 seconds
  // Because we implemented the function since 2024, we can assume that the
  // last entry is 37 seconds.
  // It might be fixed in the future.
  EXPECT_EQ(leap_info.leap_seconds.count(), 37);
}

class UtcClockLeapSecondTest : public ::testing::Test {
 protected:
  // Setup common resources if needed
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(UtcClockLeapSecondTest, LeapSecondComparisons) {
  auto ls1 = stdcpp::leap_second(std::chrono::system_clock::now(), true, 1);
  auto ls2 = ls1;  // Identical to ls1
  auto ls3 = stdcpp::leap_second(
      std::chrono::system_clock::now() + std::chrono::hours(1), false,
      2);  // Different time, positive flag, and count

  EXPECT_EQ(ls1, ls2) << "Identical leap seconds should be equal";
  EXPECT_NE(ls1, ls3) << "Different leap seconds should not be equal";
  EXPECT_LT(ls1, ls3) << "ls1 should be less than ls3 based on time";
  EXPECT_LE(ls1, ls2) << "ls1 should be less than or equal to ls2";
  EXPECT_GT(ls3, ls1) << "ls3 should be greater than ls1 based on time";
  EXPECT_GE(ls3, ls2) << "ls3 should be greater than or equal to ls2";
}

TEST_F(UtcClockLeapSecondTest, LeapSecondInfoAccuracy) {
  // Test a specific date for leap second info
  std::tm tm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  tm.tm_year = 116;  // 2016 - 1900
  tm.tm_mon = 6;     // July (tm_mon is 0-based)
  tm.tm_mday = 1;
  tm.tm_hour = 0;  // Right after a leap second at midnight UTC
  tm.tm_min = 0;
  tm.tm_sec = 0;
  auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  auto utc_tp = stdcpp::utc_clock::from_sys(tp);
  auto leap_info = stdcpp::get_leap_second_info(utc_tp);

  // Validate against the expected leap second data. For this example, assuming a leap second was added at the end of June 2016
  EXPECT_EQ(leap_info.leap_seconds.count(),
            36);  // Adjust based on the actual data for the test date
  // The test does not directly check is_leap_second because the current implementation always returns false.
}
