#ifndef __SCC_STDCPP_CHRONO_HPP__
#define __SCC_STDCPP_CHRONO_HPP__
#pragma once

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <utility>

#include <string_view.hpp>

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

template <typename Duration>
utc_time<Duration> to_utc_time(
    const std::chrono::system_clock::time_point& tp) {
  auto duration_since_epoch = tp.time_since_epoch();
  auto duration_in_target_type =
      std::chrono::duration_cast<Duration>(duration_since_epoch);
  return utc_time<Duration>(duration_in_target_type);
}

class tai_clock {
 public:
  using rep = std::chrono::system_clock::rep;
  using period = std::chrono::system_clock::period;
  using duration = std::chrono::system_clock::duration;
  using time_point = std::chrono::time_point<tai_clock>;
  static constexpr bool is_steady = std::chrono::system_clock::is_steady;

  // Get current TAI time
  static time_point now() noexcept {
    auto utc_now = std::chrono::system_clock::now();
    auto leap_info =
        get_leap_second_info(to_utc_time<std::chrono::seconds>(utc_now));
    return time_point(utc_now.time_since_epoch() + leap_info.leap_seconds);
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

// Alias template for tai_time
template <class Duration>
using tai_time = std::chrono::time_point<tai_clock, Duration>;

// Alias for tai_seconds
using tai_seconds = tai_time<std::chrono::seconds>;

// Output stream operator for tai_time
template <class charT, class traits, class Duration>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits>& os, const tai_time<Duration>& t) {
  auto sys_time = std::chrono::system_clock::time_point(
      std::chrono::duration_cast<std::chrono::system_clock::duration>(
          t.time_since_epoch()));
  std::time_t tt = std::chrono::system_clock::to_time_t(sys_time);
  os << std::ctime(&tt);
  return os;
}

// Representing chronos (C++17)
namespace representing_detail {
template <class>
constexpr bool is_duration_v = false;

template <class Rep, class Period>
constexpr bool is_duration_v<std::chrono::duration<Rep, Period>> = true;
}  // namespace representing_detail

template <class To, class Rep, class Period,
          class = std::enable_if_t<representing_detail::is_duration_v<To>>>
constexpr To floor(const std::chrono::duration<Rep, Period>& d) {
  To t = std::chrono::duration_cast<To>(d);
  if (t > d)
    return t - To{1};
  return t;
}

template <class To, class Rep, class Period,
          class = std::enable_if_t<representing_detail::is_duration_v<To>>>
constexpr To ceil(const std::chrono::duration<Rep, Period>& d) {
  To t = std::chrono::duration_cast<To>(d);
  if (t < d)
    return t + To{1};
  return t;
}

template <class To, class Rep, class Period,
          class = std::enable_if_t<representing_detail::is_duration_v<To>>>
constexpr To round(const std::chrono::duration<Rep, Period>& d) {
  To t0 = std::chrono::duration_cast<To>(d);
  To t1 = t0 + To{1};
  if (d - t0 < t1 - d)
    return t0;
  return t1;
}

using days = std::chrono::duration<int, std::ratio<86400>>;
using weeks = std::chrono::duration<int, std::ratio<604800>>;
using months = std::chrono::duration<int, std::ratio<2629746>>;
using years = std::chrono::duration<int, std::ratio<31556952>>;

template <typename Duration>
using sys_time = std::chrono::time_point<std::chrono::system_clock, Duration>;
using sys_seconds = sys_time<std::chrono::seconds>;
using sys_days = sys_time<days>;

struct local_t {};

template <class Duration>
using local_time = std::chrono::time_point<local_t, Duration>;
using local_seconds = local_time<std::chrono::seconds>;
using local_days = local_time<days>;

/**
 * @brief Usage of sys_info
 * See the cppreference: https://en.cppreference.com/w/cpp/chrono/sys_info
 * And, see https://godbolt.org/z/oME3zocoG
 */
struct sys_info {
  sys_seconds begin;
  sys_seconds end;
  std::chrono::seconds offset;
  std::chrono::minutes save;
  std::string abbrev;

  // Existing constructor
  sys_info(const sys_seconds& b, const sys_seconds& e,
           const std::chrono::seconds& o, const std::chrono::minutes& s,
           const std::string& abbr)
      : begin(b), end(e), offset(o), save(s), abbrev(abbr) {}

  // Overloaded constructor for any time_point type
  template <typename Duration>
  sys_info(
      const std::chrono::time_point<std::chrono::system_clock, Duration>& b,
      const std::chrono::time_point<std::chrono::system_clock, Duration>& e,
      const std::chrono::seconds& o, const std::chrono::minutes& s,
      const std::string& abbr)
      : begin(std::chrono::time_point_cast<std::chrono::seconds>(b)),
        end(std::chrono::time_point_cast<std::chrono::seconds>(e)),
        offset(o),
        save(s),
        abbrev(abbr) {}
};

struct local_info {
  // We prevent to use constexpr here, see:
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54483
  static const int unique = 0;
  static const int nonexistent = 1;
  static const int ambiguous = 2;

  int result;
  sys_info first;
  sys_info second;
};

class time_zone_link {
 public:
  time_zone_link(time_zone_link&&) = default;
  time_zone_link& operator=(time_zone_link&&) = default;

  // unspecified additional constructors

  string_view name() const noexcept {
    // TODO: Implement the function
    // Returns: The alternative name for the time zone.
    return "";
  }
  string_view target() const noexcept {
    // TODO: Implement the function
    // Returns: The name of the time_zone for which this time_zone_link provides an alternative name.
    return "";
  }
};

struct time_zone_info {
  std::string coordinates;
  std::string tz;
  std::string comments;
};

class time_zone {
#ifdef UNIT_TEST
  friend class TimezoneTest;
#endif
  friend struct tzdb;
  time_zone(const std::string& name, const time_zone_info& info)
      : name_(name), info_(info) {}

  std::string name_;
  time_zone_info info_;

 public:
  time_zone() = delete;
  time_zone(const time_zone&) = delete;
  time_zone& operator=(const time_zone&) = delete;
  time_zone(time_zone&&) = default;
  time_zone& operator=(time_zone&&) = default;
  ~time_zone() = default;

  const std::string& name() const { return name_; }
  template <class Duration>
  sys_info get_info(const sys_time<Duration>& st) const {
    // TODO: Implement the function
    // Because the sys_info is only accuracy to seconds (guaranteed by the standard).
    // See https://eel.is/c++draft/time.zone.info.sys
    // We can safely cast the time_point to seconds.
    auto tp_seconds = std::chrono::time_point_cast<std::chrono::seconds>(st);
    // dummy return
    return sys_info(tp_seconds, tp_seconds + std::chrono::seconds(1),
                    std::chrono::seconds(0), std::chrono::minutes(0), "UTC");
  }

  template <class Duration>
  local_info get_info(const local_time<Duration>& tp) const {
    // TODO: Implement the function

    // Because the local_info is only accuracy to seconds (guaranteed by the standard).
    // See https://eel.is/c++draft/time.zone.info.local
    // We can safely cast the time_point to seconds.
    auto tp_seconds = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    // dummy return
    return local_info{
        local_info::unique,
        sys_info(tp_seconds, tp_seconds + std::chrono::seconds(1),
                 std::chrono::seconds(0), std::chrono::minutes(0), "UTC"),
        sys_info(tp_seconds, tp_seconds + std::chrono::seconds(1),
                 std::chrono::seconds(0), std::chrono::minutes(0), "UTC")};
  }

  // to_sys
  template <class Duration>
  sys_time<Duration> to_sys(const local_time<Duration>& tp) const {
    // TODO: Implement the function
    (void)tp;

    return sys_time<Duration>();
  }

  // to_local
  template <class Duration>
  local_time<Duration> to_local(const sys_time<Duration>& tp) const {
    // TODO: Implement the function
    (void)tp;
    return local_time<Duration>();
  }

  // operator==
  friend bool operator==(const time_zone& lhs, const time_zone& rhs) {
    return lhs.name_ == rhs.name_;
  }
};

struct tzdb {
 private:
#if __cplusplus >= 201703L
  using string_view_intl = std::string_view;
#else
  using string_view_intl = stdcpp::string_view;
#endif
 public:
  std::string version;
  std::vector<time_zone> zones;
  std::vector<time_zone_link> links;
  std::vector<leap_second> leap_seconds;

  const time_zone* locate_zone(string_view_intl tz_name) const {
    // TODO: Implement the function
    for (const auto& zone : zones) {
      if (zone.name() == tz_name) {
        return &zone;
      }
    }
    return nullptr;
  }
  const time_zone* current_zone() const {
    // TODO: Implement the function
    return locate_zone("UTC");
  }
};

// Helper function to format time_point as date string
std::string format_time_point(const sys_seconds& tp) {
  std::time_t time = tp.time_since_epoch().count();
  std::tm tm = *std::gmtime(&time);  // Convert to broken-down time in UTC

  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

// Helper function to format offset as HH:MM:SS
std::string format_offset(const std::chrono::seconds& offset) {
  auto total_seconds = offset.count();
  int hours = total_seconds / 3600;
  int minutes = (total_seconds % 3600) / 60;
  int secs = total_seconds % 60;

  std::stringstream ss;
  ss << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2)
     << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0')
     << secs;
  return ss.str();
}

// Overloaded operator<< for sys_info
std::ostream& operator<<(std::ostream& os, const sys_info& info) {
  os << "[" << format_time_point(info.begin) << ","
     << format_time_point(info.end) << "," << format_offset(info.offset) << ","
     << info.save.count() << "min," << info.abbrev << "]";
  return os;
}

bool operator==(const time_zone_link& x, const time_zone_link& y) {
  return x.name() == y.name();
}
}  // namespace v1

using v1::get_leap_second_info;
using v1::leap_second;
using v1::leap_second_info;
using v1::utc_clock;
using v1::utc_seconds;
using v1::utc_time;

using v1::tai_clock;
using v1::tai_seconds;
using v1::tai_time;

using v1::to_utc_time;

}  // namespace stdcpp

#endif  // __SCC_STDCPP_CHRONO_HPP__