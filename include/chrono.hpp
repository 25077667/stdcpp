#ifndef __SCC_STDCPP_CHRONO_HPP__
#define __SCC_STDCPP_CHRONO_HPP__
#pragma once

#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>

namespace stdcpp {
namespace v1 {
class utc_clock {
 public:
  using rep = std::chrono::system_clock::rep;
  using period = std::chrono::system_clock::period;
  using duration = std::chrono::system_clock::duration;
  using time_point = std::chrono::time_point<utc_clock>;
  static constexpr bool is_steady = std::chrono::system_clock::is_steady;

  // Function to get the current time in UTC
  static time_point now() noexcept {
    return time_point(std::chrono::system_clock::now().time_since_epoch());
  }
  // to_sys
  static std::chrono::system_clock::time_point to_sys(
      const time_point& t) noexcept {
    return std::chrono::system_clock::time_point(
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            t.time_since_epoch()));
  }
  // from_sys
  static time_point from_sys(
      const std::chrono::system_clock::time_point& t) noexcept {
    return time_point(
        std::chrono::duration_cast<duration>(t.time_since_epoch()));
  }
};

// Alias template for utc_time
template <class Duration>
using utc_time = std::chrono::time_point<utc_clock, Duration>;

// Alias for utc_seconds
using utc_seconds = utc_time<std::chrono::seconds>;

// Output stream operator for utc_time
template <class charT, class traits, class Duration>
static std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits>& os, const utc_time<Duration>& t) {
  auto sys_time = utc_clock::to_sys(t);
  std::time_t tt = std::chrono::system_clock::to_time_t(sys_time);
  os << std::ctime(&tt);
  return os;
}

// Placeholder for leap_second_info structure
struct leap_second_info {
  bool is_leap_second;
  std::chrono::seconds leap_seconds;
  // Constructor
  leap_second_info(bool leap = false,
                   std::chrono::seconds sec = std::chrono::seconds(0))
      : is_leap_second(leap), leap_seconds(sec) {}
};

// Placeholder class for leap_second
struct leap_second {
  std::chrono::time_point<std::chrono::system_clock> time;
  bool positive;
  int count;

  // Constructor
  leap_second(const std::chrono::time_point<std::chrono::system_clock>& time,
              bool positive, int count)
      : time(time), positive(positive), count(count) {}

  // Comparison operators
  friend bool operator==(const leap_second& lhs, const leap_second& rhs) {
    return lhs.time == rhs.time && lhs.positive == rhs.positive &&
           lhs.count == rhs.count;
  }

  friend bool operator<(const leap_second& lhs, const leap_second& rhs) {
    return lhs.time < rhs.time;
  }
  friend bool operator>(const leap_second& lhs, const leap_second& rhs) {
    return rhs < lhs;
  }
  friend bool operator!=(const leap_second& lhs, const leap_second& rhs) {
    return !(lhs == rhs);
  }
  friend bool operator<=(const leap_second& lhs, const leap_second& rhs) {
    return !(lhs > rhs);
  }
  friend bool operator>=(const leap_second& lhs, const leap_second& rhs) {
    return !(lhs < rhs);
  }
};

template <class Duration>
static leap_second_info get_leap_second_info(const utc_time<Duration>& ut) {
  // Leap seconds data (MJD, TAI-UTC)
  static const std::array<std::pair<long, int>, 28> leap_seconds_data = {
      std::make_pair(41317, 10), std::make_pair(41499, 11),
      std::make_pair(41683, 12), std::make_pair(42048, 13),
      std::make_pair(42413, 14), std::make_pair(42778, 15),
      std::make_pair(43144, 16), std::make_pair(43509, 17),
      std::make_pair(43874, 18), std::make_pair(44239, 19),
      std::make_pair(44786, 20), std::make_pair(45151, 21),
      std::make_pair(45516, 22), std::make_pair(46247, 23),
      std::make_pair(47161, 24), std::make_pair(47892, 25),
      std::make_pair(48257, 26), std::make_pair(48804, 27),
      std::make_pair(49169, 28), std::make_pair(49534, 29),
      std::make_pair(50083, 30), std::make_pair(50630, 31),
      std::make_pair(51179, 32), std::make_pair(53736, 33),
      std::make_pair(54832, 34), std::make_pair(56109, 35),
      std::make_pair(57204, 36), std::make_pair(57754, 37),
  };

  // Convert utc_time to time_t for comparison
  auto sys_time = utc_clock::to_sys(ut);
  std::time_t tt = std::chrono::system_clock::to_time_t(sys_time);
  std::tm* tm_utc = std::gmtime(&tt);

  // Convert tm to Modified Julian Date (MJD)
  int y = tm_utc->tm_year + 1900;
  int m = tm_utc->tm_mon + 1;
  int d = tm_utc->tm_mday;
  if (m <= 2) {
    m += 12;
    y -= 1;
  }
  long mjd = static_cast<long>(365.25 * y) - 679006 +
             static_cast<long>(30.6001 * (m + 1)) + d;

  // Find the last leap second before or equal to the given utc_time
  int last_leap_second = 0;
  for (const auto& pair : leap_seconds_data) {
    if (mjd >= pair.first) {
      last_leap_second = pair.second;
    } else {
      break;
    }
  }

  return leap_second_info(false, std::chrono::seconds(last_leap_second));
}
}  // namespace v1

using v1::get_leap_second_info;
using v1::leap_second;
using v1::leap_second_info;
using v1::utc_clock;
using v1::utc_seconds;
using v1::utc_time;

}  // namespace stdcpp

#endif  // __SCC_STDCPP_CHRONO_HPP__