#ifndef __SCC_STDCPP_STRING_VIEW_HPP__
#define __SCC_STDCPP_STRING_VIEW_HPP__
#pragma once

#if __cplusplus < 201703L
#include <string.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace stdcpp {
namespace v1 {

template <typename CharT, typename Traits = std::char_traits<CharT>>
class basic_string_view {

 public:
  // types
  using traits_type = Traits;
  using value_type = CharT;
  using pointer = CharT*;
  using const_pointer = const CharT*;
  using reference = CharT&;
  using const_reference = const CharT&;
  using const_iterator = const CharT*;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  // we couldn't use constexpr here because of the C++14 compatibility
  // See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54483
  static size_type npos;

 private:
  const_pointer data_;
  size_type size_;

 public:
  // [string.view.cons], construction and assignment
  constexpr basic_string_view() noexcept : data_(nullptr), size_(0) {}
  constexpr basic_string_view(const basic_string_view&) noexcept = default;
  constexpr basic_string_view& operator=(const basic_string_view&) noexcept =
      default;
  constexpr basic_string_view(const CharT* str)
      : data_(str), size_(Traits::length(str)) {}
  basic_string_view(nullptr_t) = delete;
  constexpr basic_string_view(const CharT* str, size_type len)
      : data_(str), size_(len) {}

  template <class It, class End,
            class = std::enable_if_t<std::is_convertible<
                typename std::iterator_traits<It>::iterator_category,
                std::random_access_iterator_tag>::value>>
  constexpr basic_string_view(It begin, End end)
      : data_(begin), size_(std::distance(begin, end)) {}

  // Explicit conversion from another range type
  template <class R>
  constexpr explicit basic_string_view(R&& r)
      : data_(r.data()), size_(r.size()) {}

 public:
  constexpr const_reference at(size_type pos) const {
    if (pos >= size_)
      throw std::out_of_range("basic_string_view::at");
    return data_[pos];
  }
  constexpr const_pointer data() const noexcept { return data_; }
  constexpr const_reference front() const { return *data_; }
  constexpr const_reference back() const { return *(data_ + size_ - 1); }
  constexpr bool empty() const noexcept { return size_ == 0; }

  // Iterators
  // [string.view.iterators], iterator support
  constexpr const_iterator begin() const noexcept { return data_; }
  constexpr const_iterator end() const noexcept { return data_ + size_; }
  constexpr const_iterator cbegin() const noexcept { return begin(); }
  constexpr const_iterator cend() const noexcept { return end(); }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  constexpr const_reverse_iterator crend() const noexcept { return rend(); }

  // [string.view.capacity], capacity
  constexpr size_type size() const noexcept { return size_; }
  constexpr size_type length() const noexcept { return size_; }
  constexpr size_type max_size() const noexcept { return npos - 1; }

  // [string.view.access], element access
  constexpr const_reference operator[](size_type pos) const {
    return data_[pos];
  }

  // [string.view.modifiers], modifiers
  constexpr void remove_prefix(size_type n) {
    data_ += n;
    size_ -= n;
  }
  constexpr void remove_suffix(size_type n) { size_ -= n; }
  constexpr void swap(basic_string_view& s) noexcept {
    auto temp_data = data_;
    auto temp_size = size_;
    data_ = s.data_;
    size_ = s.size_;
    s.data_ = temp_data;
    s.size_ = temp_size;
  }

  // [string.view.ops], string operations
  constexpr size_type copy(CharT* s, size_type n, size_type pos = 0) const {
    if (pos > size_)
      throw std::out_of_range("basic_string_view::copy");
    const size_type len = std::min(n, size_ - pos);
    Traits::copy(s, data_ + pos, len);
    return len;
  }

  constexpr basic_string_view substr(size_type pos = 0,
                                     size_type n = npos) const {
    if (pos > size_)
      throw std::out_of_range("basic_string_view::substr");
    const size_type len = std::min(n, size_ - pos);
    return basic_string_view(data_ + pos, len);
  }

  // operator==
  friend constexpr bool operator==(basic_string_view x,
                                   basic_string_view y) noexcept {
    return x.size_ == y.size_ &&
           Traits::compare(x.data_, y.data_, x.size_) == 0;
  }

  friend constexpr bool operator!=(basic_string_view x,
                                   basic_string_view y) noexcept {
    return !(x == y);
  }

  friend constexpr bool operator<(basic_string_view x,
                                  basic_string_view y) noexcept {
    const size_type len = std::min(x.size_, y.size_);
    const int result = Traits::compare(x.data_, y.data_, len);
    return result < 0 || (result == 0 && x.size_ < y.size_);
  }

  friend constexpr bool operator>(basic_string_view x,
                                  basic_string_view y) noexcept {
    return y < x;
  }

  friend constexpr bool operator<=(basic_string_view x,
                                   basic_string_view y) noexcept {
    return !(y < x);
  }

  friend constexpr bool operator>=(basic_string_view x,
                                   basic_string_view y) noexcept {
    return !(x < y);
  }

  constexpr int compare(basic_string_view s) const noexcept {
    const size_type len1 = size_, len2 = s.size_, rlen = std::min(len1, len2);
    int result = Traits::compare(data_, s.data_, rlen);
    if (result != 0)
      return result;
    if (len1 == len2)
      return 0;
    return len1 < len2 ? -1 : 1;
  }

  constexpr int compare(size_type pos1, size_type n1,
                        basic_string_view s) const {
    return substr(pos1, n1).compare(s);
  }

  constexpr int compare(size_type pos1, size_type n1, basic_string_view s,
                        size_type pos2, size_type n2) const {
    return substr(pos1, n1).compare(s.substr(pos2, n2));
  }

  constexpr int compare(const CharT* s) const {
    return compare(basic_string_view(s));
  }

  constexpr int compare(size_type pos1, size_type n1, const CharT* s) const {
    return substr(pos1, n1).compare(basic_string_view(s));
  }

  constexpr int compare(size_type pos1, size_type n1, const CharT* s,
                        size_type n2) const {
    return substr(pos1, n1).compare(basic_string_view(s, n2));
  }

  // Check if the string_view starts with the given prefix
  constexpr bool starts_with(basic_string_view x) const noexcept {
    return size_ >= x.size_ && compare(0, x.size_, x) == 0;
  }

  constexpr bool starts_with(CharT x) const noexcept {
    return !empty() && Traits::eq(front(), x);
  }

  constexpr bool starts_with(const CharT* x) const {
    return starts_with(basic_string_view(x));
  }

  // Check if the string_view ends with the given suffix
  constexpr bool ends_with(basic_string_view x) const noexcept {
    return size_ >= x.size_ && compare(size_ - x.size_, npos, x) == 0;
  }

  constexpr bool ends_with(CharT x) const noexcept {
    return !empty() && Traits::eq(back(), x);
  }

  constexpr bool ends_with(const CharT* x) const {
    return ends_with(basic_string_view(x));
  }

  // Check if the string_view contains the given substring
  constexpr bool contains(basic_string_view x) const noexcept {
    return find(x) != npos;
  }

  constexpr bool contains(CharT x) const noexcept { return find(x) != npos; }

  constexpr bool contains(const CharT* x) const {
    return find(basic_string_view(x)) != npos;
  }

  template <size_type N>
  constexpr bool contains(const CharT (&x)[N]) const {
    return find(basic_string_view(x, N - 1)) != npos;
  }

  // [string.view.find], searching
  constexpr size_type find(basic_string_view s,
                           size_type pos = 0) const noexcept {
    if (pos > size_ || s.size_ > size_ - pos)
      return npos;
    if (s.size_ == 0)
      return pos;
    for (size_type i = pos; i <= size_ - s.size_; ++i) {
      if (Traits::compare(data_ + i, s.data_, s.size_) == 0) {
        return i;
      }
    }
    return npos;
  }

  constexpr size_type find(CharT c, size_type pos = 0) const noexcept {
    if (pos >= size_)
      return npos;
    for (size_type i = pos; i < size_; ++i) {
      if (Traits::eq(data_[i], c)) {
        return i;
      }
    }
    return npos;
  }

  constexpr size_type find(const CharT* s, size_type pos, size_type n) const {
    return find(basic_string_view(s, n), pos);
  }

  constexpr size_type find(const CharT* s, size_type pos = 0) const {
    return find(basic_string_view(s), pos);
  }

  constexpr size_type rfind(basic_string_view s,
                            size_type pos = npos) const noexcept {
    pos = std::min(pos, size_);
    if (s.size_ > size_)
      return npos;
    if (s.size_ == 0)
      return pos;

    for (size_type i = pos + 1; i >= s.size_; --i) {
      if (Traits::compare(data_ + i - s.size_, s.data_, s.size_) == 0) {
        return i - s.size_;
      }
      if (i == 0)
        break;  // Prevent underflow
    }
    return npos;
  }

  constexpr size_type rfind(CharT c, size_type pos = npos) const noexcept {
    pos = std::min(pos, size_ - 1);
    for (size_type i = pos + 1; i > 0; --i) {
      if (Traits::eq(data_[i - 1], c)) {
        return i - 1;
      }
    }
    return npos;
  }

  constexpr size_type rfind(const CharT* s, size_type pos, size_type n) const {
    return rfind(basic_string_view(s, n), pos);
  }

  constexpr size_type rfind(const CharT* s, size_type pos = npos) const {
    return rfind(basic_string_view(s), pos);
  }

  // Find the first character from the set
  constexpr size_type find_first_of(basic_string_view s,
                                    size_type pos = 0) const noexcept {
    for (size_type i = pos; i < size_; ++i) {
      if (s.find(data_[i]) != npos) {
        return i;
      }
    }
    return npos;
  }

  constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept {
    return find(c, pos);
  }

  constexpr size_type find_first_of(const CharT* s, size_type pos,
                                    size_type n) const {
    return find_first_of(basic_string_view(s, n), pos);
  }

  constexpr size_type find_first_of(const CharT* s, size_type pos = 0) const {
    return find_first_of(basic_string_view(s), pos);
  }

  // Find the last character from the set
  constexpr size_type find_last_of(basic_string_view s,
                                   size_type pos = npos) const noexcept {
    pos = std::min(pos, size_ - 1);
    for (size_type i = pos + 1; i > 0; --i) {
      if (s.find(data_[i - 1]) != npos) {
        return i - 1;
      }
    }
    return npos;
  }

  constexpr size_type find_last_of(CharT c,
                                   size_type pos = npos) const noexcept {
    return rfind(c, pos);
  }

  constexpr size_type find_last_of(const CharT* s, size_type pos,
                                   size_type n) const {
    return find_last_of(basic_string_view(s, n), pos);
  }

  constexpr size_type find_last_of(const CharT* s, size_type pos = npos) const {
    return find_last_of(basic_string_view(s), pos);
  }

  // Find the first character not in the set
  constexpr size_type find_first_not_of(basic_string_view s,
                                        size_type pos = 0) const noexcept {
    for (size_type i = pos; i < size_; ++i) {
      if (s.find(data_[i]) == npos) {
        return i;
      }
    }
    return npos;
  }

  constexpr size_type find_first_not_of(CharT c,
                                        size_type pos = 0) const noexcept {
    for (size_type i = pos; i < size_; ++i) {
      if (!Traits::eq(data_[i], c)) {
        return i;
      }
    }
    return npos;
  }

  constexpr size_type find_first_not_of(const CharT* s, size_type pos,
                                        size_type n) const {
    return find_first_not_of(basic_string_view(s, n), pos);
  }

  constexpr size_type find_first_not_of(const CharT* s,
                                        size_type pos = 0) const {
    return find_first_not_of(basic_string_view(s), pos);
  }

  // Find the last character not in the set
  constexpr size_type find_last_not_of(basic_string_view s,
                                       size_type pos = npos) const noexcept {
    pos = std::min(pos, size_ - 1);
    for (size_type i = pos + 1; i > 0; --i) {
      if (s.find(data_[i - 1]) == npos) {
        return i - 1;
      }
    }
    return npos;
  }

  constexpr size_type find_last_not_of(CharT c,
                                       size_type pos = npos) const noexcept {
    for (size_type i = pos + 1; i > 0; --i) {
      if (!Traits::eq(data_[i - 1], c)) {
        return i - 1;
      }
    }
    return npos;
  }

  constexpr size_type find_last_not_of(const CharT* s, size_type pos,
                                       size_type n) const {
    return find_last_not_of(basic_string_view(s, n), pos);
  }

  constexpr size_type find_last_not_of(const CharT* s,
                                       size_type pos = npos) const {
    return find_last_not_of(basic_string_view(s), pos);
  }

  // Non-member functions (comparison operators, etc.) should be implemented outside the class.
};

template <typename CharT, typename Traits>
typename basic_string_view<CharT, Traits>::size_type
    basic_string_view<CharT, Traits>::npos = static_cast<size_type>(-1);
};  // namespace v1

// Suffix for basic_string_view literals
inline namespace literals {
inline namespace string_view_literals {
constexpr v1::basic_string_view<char> operator"" _sv(const char* str,
                                                     std::size_t len) {
  return v1::basic_string_view<char>(str, len);
}

constexpr v1::basic_string_view<wchar_t> operator"" _sv(const wchar_t * str,
                                                        std::size_t len) {
  return v1::basic_string_view<wchar_t>(str, len);
}

constexpr v1::basic_string_view<char16_t> operator"" _sv(const char16_t* str,
                                                         std::size_t len) {
  return v1::basic_string_view<char16_t>(str, len);
}

constexpr v1::basic_string_view<char32_t> operator"" _sv(const char32_t* str,
                                                         std::size_t len) {
  return v1::basic_string_view<char32_t>(str, len);
}

}  // namespace string_view_literals
}  // namespace literals

using string_view = v1::basic_string_view<char>;
using wstring_view = v1::basic_string_view<wchar_t>;
using u16string_view = v1::basic_string_view<char16_t>;
using u32string_view = v1::basic_string_view<char32_t>;

using namespace literals::string_view_literals;
}  // namespace stdcpp

// Open the std namespace to add specializations
namespace std {

template <>
struct hash<stdcpp::v1::basic_string_view<char>> {
  size_t operator()(stdcpp::v1::basic_string_view<char> sv) const noexcept {
    return hash<std::string>()(std::string(sv.data(), sv.size()));
  }
};

template <>
struct hash<stdcpp::v1::basic_string_view<wchar_t>> {
  size_t operator()(stdcpp::v1::basic_string_view<wchar_t> sv) const noexcept {
    return hash<std::wstring>()(std::wstring(sv.data(), sv.size()));
  }
};

// char16_t
template <>
struct hash<stdcpp::v1::basic_string_view<char16_t>> {
  size_t operator()(stdcpp::v1::basic_string_view<char16_t> sv) const noexcept {
    return hash<std::basic_string<char16_t>>()(
        std::basic_string<char16_t>(sv.data(), sv.size()));
  }
};

// Add the missing char32_t specialization
template <>
struct hash<stdcpp::v1::basic_string_view<char32_t>> {
  size_t operator()(stdcpp::v1::basic_string_view<char32_t> sv) const noexcept {
    return hash<std::basic_string<char32_t>>()(
        std::basic_string<char32_t>(sv.data(), sv.size()));
  }
};

}  // namespace std

#else
#include <string_view>
namespace stdcpp {
using std::string_view;
}  // namespace stdcpp

#endif
#endif  // __SCC_STDCPP_STRING_VIEW_HPP__