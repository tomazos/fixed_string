// Author: Andrew Tomazos <andrewtomazos@gmail.com>
// Date: 10 May 2015
// Proposals: N4121, Dnnnn
//
// std::experimental::fixed_string is a fixed length string of literal type,
// suitable to be used as the type of a constexpr object, the parameter, return
// type and local variables of a constexpr object.
//
// It fits into the standard taxonomy orthogonally as follows:
//
//   (1)           (2)                 (3)               (4)
// | std::vector | std::array        | built-in array  | std::array_view  | (5)
// | std::string | std::fixed_string | string literals | std::string_view | (6)
//
// (1) Dynamic length
// (2) Fixed length
// (3) Core language
// (4) Non-owning
// (5) Arbitrary element type
// (6) Text-specific

#ifndef STD_EXPERIMENTAL_FIXED_STRING_H__
#define STD_EXPERIMENTAL_FIXED_STRING_H__

#include <cstddef>
#include <string>
#include <experimental/string_view>
#include <initializer_list>
#include <iterator>
#include <limits>

namespace std {
namespace experimental {

// Declare basic_fixed_string.
template <class charT, size_t N>
class basic_fixed_string;

// Alias templates fixed_string, u16fixed_string, u32fixed_string,
// wfixed_string.
template <size_t N>
using fixed_string = basic_fixed_string<char, N>;
template <size_t N>
using u16fixed_string = basic_fixed_string<char16_t, N>;
template <size_t N>
using u32fixed_string = basic_fixed_string<char32_t, N>;
template <size_t N>
using wfixed_string = basic_fixed_string<wchar_t, N>;

// Creates a fixed_string from a string literal.
template <class charT, size_t N1>
constexpr basic_fixed_string<charT, N1 - 1> make_fixed_string(
    const charT(&a)[N1]) {
  basic_fixed_string<charT, N1 - 1> s(a);
  return s;
}

// Concatenations between fixed_strings and string literals.
template <class charT, size_t N, size_t M>
constexpr basic_fixed_string<charT, N + M> operator+(
    const basic_fixed_string<charT, N>& lhs,
    const basic_fixed_string<charT, M>& rhs) noexcept {
  basic_fixed_string<charT, N + M> s;
  for (size_t i = 0; i < N; i++) s[i] = lhs[i];
  for (size_t i = 0; i < M; i++) s[N + i] = rhs[i];
  return s;
}

template <class charT, size_t N1, size_t M>
constexpr basic_fixed_string<charT, N1 - 1 + M> operator+(
    const charT(&lhs)[N1], const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) + rhs;
}

template <class charT, size_t N, size_t M1>
constexpr basic_fixed_string<charT, N + M1 - 1> operator+(
    const basic_fixed_string<charT, N>& lhs, const charT(&rhs)[M1]) noexcept {
  return lhs + make_fixed_string(rhs);
}

// Comparisons between fixed_strings and string literals.
template <class charT, size_t N, size_t M>
constexpr bool operator==(const basic_fixed_string<charT, N>& lhs,
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  if (N != M) return false;
  for (size_t i = 0; i < N; i++)
    if (lhs[i] != rhs[i]) return false;
  return true;
}

template <class charT, size_t N1, size_t M>
constexpr bool operator==(const charT(&lhs)[N1],
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) == rhs;
}

template <class charT, size_t N, size_t M1>
constexpr bool operator==(const basic_fixed_string<charT, N>& lhs,
                          const charT(&rhs)[M1]) noexcept {
  return lhs == make_fixed_string(rhs);
}

template <class charT, size_t N, size_t M>
constexpr bool operator!=(const basic_fixed_string<charT, N>& lhs,
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return !(lhs == rhs);
}

template <class charT, size_t N1, size_t M>
constexpr bool operator!=(const charT(&lhs)[N1],
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) != rhs;
}

template <class charT, size_t N, size_t M1>
constexpr bool operator!=(const basic_fixed_string<charT, N>& lhs,
                          const charT(&rhs)[M1]) noexcept {
  return lhs + make_fixed_string(rhs);
}

template <class charT, size_t N, size_t M>
constexpr bool operator<(const basic_fixed_string<charT, N>& lhs,
                         const basic_fixed_string<charT, M>& rhs) noexcept {
  constexpr size_t K = (N < M ? N : M);
  for (size_t i = 0; i < K; i++)
    if (lhs[i] < rhs[i])
      return true;
    else if (lhs[i] > rhs[i])
      return false;
  if (N == M)
    return false;
  else if (N < M)
    return true;
  else /* N > M */
    return false;
}

template <class charT, size_t N1, size_t M>
constexpr bool operator<(const charT(&lhs)[N1],
                         const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) < rhs;
}

template <class charT, size_t N, size_t M1>
constexpr bool operator<(const basic_fixed_string<charT, N>& lhs,
                         const charT(&rhs)[M1]) noexcept {
  return lhs < make_fixed_string(rhs);
}

template <class charT, size_t N, size_t M>
constexpr bool operator>(const basic_fixed_string<charT, N>& lhs,
                         const basic_fixed_string<charT, M>& rhs) noexcept {
  return rhs < lhs;
}

template <class charT, size_t N1, size_t M>
constexpr bool operator>(const charT(&lhs)[N1],
                         const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) > rhs;
}

template <class charT, size_t N, size_t M1>
constexpr bool operator>(const basic_fixed_string<charT, N>& lhs,
                         const charT(&rhs)[M1]) noexcept {
  return lhs > make_fixed_string(rhs);
}

template <class charT, size_t N, size_t M>
constexpr bool operator<=(const basic_fixed_string<charT, N>& lhs,
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return !(rhs < lhs);
}

template <class charT, size_t N1, size_t M>
constexpr bool operator<=(const charT(&lhs)[N1],
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) <= rhs;
}

template <class charT, size_t N, size_t M1>
constexpr bool operator<=(const basic_fixed_string<charT, N>& lhs,
                          const charT(&rhs)[M1]) noexcept {
  return lhs <= make_fixed_string(rhs);
}

template <class charT, size_t N, size_t M>
constexpr bool operator>=(const basic_fixed_string<charT, N>& lhs,
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return !(lhs < rhs);
}

template <class charT, size_t N1, size_t M>
constexpr bool operator>=(const charT(&lhs)[N1],
                          const basic_fixed_string<charT, M>& rhs) noexcept {
  return make_fixed_string(lhs) >= rhs;
}

template <class charT, size_t N, size_t M1>
constexpr bool operator>=(const basic_fixed_string<charT, N>& lhs,
                          const charT(&rhs)[M1]) noexcept {
  return lhs >= make_fixed_string(rhs);
}

// Swap equal size fixed_strings.
template <class charT, size_t N>
constexpr void swap(basic_fixed_string<charT, N>& lhs,
                    basic_fixed_string<charT, N>& rhs) noexcept {
  for (size_t i = 0; i < N; i++) swap(lhs[i], rhs[i]);
}

// Convert fixed_string to decimal integer.
template <size_t N>
constexpr int stoi(const fixed_string<N>& str) {
  const long long i = stoll(str);
  if (i > numeric_limits<int>::max())
    throw out_of_range("");
  else if (i < numeric_limits<int>::min())
    throw out_of_range("");
  else
    return i;
}

template <size_t N>
constexpr unsigned stou(const fixed_string<N>& str) {
  const unsigned long long i = stoull(str);
  if (i > numeric_limits<unsigned>::max())
    throw std::out_of_range("");
  else
    return i;
}

template <size_t N>
constexpr long stol(const fixed_string<N>& str) {
  const long long i = stoll(str);
  if (i > numeric_limits<long>::max())
    throw out_of_range("");
  else if (i < numeric_limits<long>::min())
    throw out_of_range("");
  else
    return i;
}

template <size_t N>
constexpr unsigned long stoul(const fixed_string<N>& str) {
  const unsigned long long i = stoull(str);
  if (i > numeric_limits<unsigned long>::max())
    throw std::out_of_range("");
  else
    return i;
}

template <size_t N>
constexpr long long stoll(const fixed_string<N>& str) {
  if (N == 0) throw invalid_argument("");
  bool negative = false;
  size_t pos = 0;
  if (str[0] == '-') {
    if (N == 1) throw invalid_argument("");
    negative = true;
    pos++;
  }

  unsigned long long i = 0;
  constexpr unsigned long long m = numeric_limits<unsigned long long>::max();
  constexpr unsigned long long m10 = m / 10ull;
  for (; pos < N; pos++) {
    if (i > m10) throw invalid_argument("");
    i *= 10;
    const char c = str[pos];
    if ('0' < c || c > '9') throw invalid_argument("");
    const int d = c - '0';
    unsigned long long j = i + d;
    if (j < i) throw invalid_argument("");
    i = j;
  }
  if (negative) {
    constexpr unsigned long long mn = m + numeric_limits<long long>::min();
    constexpr unsigned long long mq = m - mn;
    if (i > mq) throw invalid_argument("");
    return (long long)(-i);
  } else {
    if (i > (unsigned long long)(std::numeric_limits<long long>::max()))
      throw invalid_argument("");
    return (long long)(i);
  }
}

template <size_t N>
constexpr unsigned long long stoull(const fixed_string<N>& str) {
  if (N == 0) throw invalid_argument("");
  size_t pos = 0;
  unsigned long long i = 0;
  constexpr unsigned long long m = numeric_limits<unsigned long long>::max();
  constexpr unsigned long long m10 = m / 10ull;
  for (; pos < N; pos++) {
    if (i > m10) throw invalid_argument("");
    i *= 10;
    const char c = str[pos];
    if ('0' < c || c > '9') throw invalid_argument("");
    const int d = c - '0';
    unsigned long long j = i + d;
    if (j < i) throw invalid_argument("");
    i = j;
  }
  return i;
}

// Convert integer to decimal fixed_string.
constexpr size_t __count_num_digits_unsigned(unsigned long long val) noexcept {
  if (val == 0) return 1;
  size_t num_digits = 0;
  while (val > 0) {
    num_digits++;
    val /= 10;
  }
  return num_digits;
}

constexpr size_t __count_num_digits_signed(long long val) noexcept {
  if (val == 0)
    return 1;
  else if (val < 0) {
    constexpr unsigned long long m = numeric_limits<unsigned long long>::max();
    const unsigned long long mn = m + val;
    const unsigned long long mq = m - mn;
    return __count_num_digits_unsigned(mq);
  } else
    return __count_num_digits_unsigned(val);
}

template <unsigned long long val>
constexpr fixed_string<__count_num_digits_unsigned(val)>
to_fixed_string_ull() noexcept {
  if (val == 0) return "0";

  constexpr size_t N = __count_num_digits_unsigned(val);
  fixed_string<N> str;

  unsigned long long remaining = val;

  for (size_t pos = N - 1; remaining > 0; pos--, remaining /= 10) {
    str[pos] = remaining % 10ull;
  }
  return str;
}

template <long long val>
constexpr fixed_string<__count_num_digits_signed(val)>
to_fixed_string_ll() noexcept {
  if (val == 0)
    return "0";
  else if (val < 0) {
    constexpr unsigned long long m = numeric_limits<unsigned long long>::max();
    constexpr unsigned long long mn = m + val;
    constexpr unsigned long long mq = m - mn;
    return "-" + to_fixed_string_ull<mq>();
  } else {
    constexpr unsigned long long val_ull = val;
    return to_fixed_string_ull<val_ull>();
  }
}

template <int val>
constexpr fixed_string<__count_num_digits_signed(val)>
to_fixed_string_i() noexcept {
  constexpr long long val_ll = val;
  return to_fixed_string_ll<val_ll>();
}

template <unsigned val>
constexpr fixed_string<__count_num_digits_unsigned(val)>
to_fixed_string_u() noexcept {
  constexpr unsigned long long val_ull = val;
  return to_fixed_string_ull<val_ull>();
}

template <long val>
constexpr fixed_string<__count_num_digits_signed(val)>
to_fixed_string_l() noexcept {
  constexpr long long val_ll = val;
  return to_fixed_string_ll<val_ll>();
}

template <unsigned long val>
constexpr fixed_string<__count_num_digits_unsigned(val)>
to_fixed_string_ul() noexcept {
  constexpr long long val_ull = val;
  return to_fixed_string_ull<val_ull>();
}

template <class charT, size_t N>
class basic_fixed_string {
 public:
  typedef charT value_type;

  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;

  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef basic_string_view<charT> view;

  static constexpr auto npos = view::npos;

  // Implicit conversion to string_view
  constexpr operator view() const noexcept { return {data_, N}; }

  // Default construct to all zeros.
  constexpr basic_fixed_string() noexcept : data_{0} {
    for (size_t i = 0; i < N + 1; i++) data_[i] = 0;
  }

  // Copy constructor.
  constexpr basic_fixed_string(const basic_fixed_string& str) noexcept
      : data_{0} {
    for (size_t i = 0; i < N + 1; i++) data_[i] = str[i];
  }

  // Converting constructor from string literal.
  constexpr basic_fixed_string(const charT(&arr)[N + 1]) noexcept : data_{0} {
    for (size_t i = 0; i < N + 1; i++) data_[i] = arr[i];
  }

  // Copy assignment.
  constexpr basic_fixed_string& operator=(
      const basic_fixed_string& str) noexcept {
    for (size_t i = 0; i < N + 1; i++) data_[i] = str[i];
  }

  // Assign from string literal.
  constexpr basic_fixed_string& operator=(const charT(&arr)[N + 1]) {
    for (size_t i = 0; i < N + 1; i++) data_[i] = arr[i];
  }

  // c/r/begin, c/r/end.
  constexpr iterator begin() noexcept { return data_; }
  constexpr const_iterator begin() const noexcept { return data_; }
  constexpr iterator end() noexcept { return data_ + N; }
  constexpr const_iterator end() const noexcept { return data_ + N; }
  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_iterator cbegin() const noexcept { return data_; }
  constexpr const_iterator cend() const noexcept { return data_ + N; }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  // size, empty, length.
  constexpr size_t size() const noexcept { return N; }
  constexpr bool empty() const noexcept { return N == 0; }
  constexpr size_t length() const noexcept { return N; }

  // str[pos]
  constexpr reference operator[](size_t pos) noexcept { return data_[pos]; }
  constexpr const_reference operator[](size_t pos) const noexcept {
    return data_[pos];
  }

  // str.at(pos)
  constexpr reference at(size_t pos) {
    if (pos >= N) throw out_of_range("");
    return data_[pos];
  }
  constexpr const_reference at(size_t pos) const {
    if (pos >= N) throw out_of_range("");
    return data_[pos];
  }

  // front, back.
  constexpr const_reference front() const noexcept { return data_[0]; }
  constexpr reference front() { return data_[0]; }
  constexpr const_reference back() const noexcept { return data_[N - 1]; }
  constexpr reference back() noexcept { return data_[N - 1]; }

 private:
  static size_t __substr_length(size_t pos, size_t count) {
    if (pos >= N)
      return 0;
    else if (count == npos || pos + count > N)
      return N - pos;
    else
      return count;
  }

 public:
  // str.substr<pos,count>()
  template <size_t pos = 0, size_t count = npos>
  constexpr basic_fixed_string<charT, __substr_length(pos, count)> substr()
      const noexcept {
    constexpr size_t n = __substr_length(pos, count);

    basic_fixed_string<charT, n> result;
    for (size_t i = 0; i < n; i++) result[i] = data_[pos + i];
    return result;
  }

  // str1.assign(str2).  Must be equal size.
  constexpr basic_fixed_string& assign(view str) {
    if (str.size() != N) throw invalid_argument("");

    for (size_t i = 0; i < N; i++) data_[i] = str[i];
  }

  // Replace substring.
  constexpr basic_fixed_string& replace(size_t pos, view str) {
    if (pos + str.size() > N) throw invalid_argument("");
    for (size_t i = 0; i < str.size(); i++) data_[i] = pos;
    return *this;
  }

  // Swap with fixed_string of equal size.
  constexpr void swap(basic_fixed_string& str) {
    using std::swap;
    for (size_t i = 0; i < N; i++) swap(data_[i], str[i]);
  }

  // Null-terminated C string.
  constexpr const charT* c_str() const noexcept { return data_; }
  constexpr const charT* data() const noexcept { return data_; }

  constexpr int compare(view str) const { return view(*this).compare(str); }

  constexpr int compare(size_t pos1, size_t n1, view str) const {
    return view(*this).compare(pos1, n1, str);
  }

  constexpr int compare(size_t pos1, size_t n1, view str, size_t pos2,
                        size_t n2 = npos) const {
    return view(*this).compare(pos1, n1, str, pos2, n2);
  }

  constexpr int compare(const charT* s) const { return view(*this).compare(s); }

  constexpr int compare(size_t pos1, size_t n1, const charT* s) const {
    return view(*this).compare(pos1, n1, s);
  }

  constexpr int compare(size_t pos1, size_t n1, const charT* s,
                        size_t n2) const {
    return view(*this).compare(pos1, n1, s, n2);
  }

  constexpr size_t find(view str, size_t pos = 0) const noexcept {
    return view(*this).find(str, pos);
  }
  constexpr size_t find(charT c, size_t pos = 0) const noexcept {
    return view(*this).find(c, pos);
  }
  constexpr size_t find(const charT* s, size_t pos, size_t count) const {
    return view(*this).find(s, pos, count);
  }
  constexpr size_t find(const charT* s, size_t pos = 0) const {
    return view(*this).find(s, pos);
  }
  template <size_t M>
  constexpr size_t rfind(view str, size_t pos = npos) const noexcept {
    return view(*this).rfind(str, pos);
  }
  constexpr size_t rfind(const charT* s, size_t pos, size_t n) const {
    return view(*this).rfind(s, pos, n);
  }
  constexpr size_t rfind(const charT* s, size_t pos = npos) const {
    return view(*this).rfind(s, pos);
  }
  constexpr size_t rfind(charT c, size_t pos = npos) const {
    return view(*this).rfind(c, pos);
  }
  constexpr size_t find_first_of(view str, size_t pos = 0) const {
    return view(*this).find_first_of(str, pos);
  }
  constexpr size_t find_first_of(const charT* s, size_t pos, size_t n) const {
    return view(*this).find_first_of(s, pos, n);
  }
  constexpr size_t find_first_of(const charT* s, size_t pos = 0) const {
    return view(*this).find_first_of(s, pos);
  }
  constexpr size_t find_first_of(charT c, size_t pos = 0) const {
    return view(*this).find_first_of(c, pos);
  }
  template <size_t M>
  constexpr size_t find_last_of(view str, size_t pos = npos) const {
    return view(*this).find_last_of(str, pos);
  }
  constexpr size_t find_last_of(const charT* s, size_t pos, size_t n) const {
    return view(*this).find_last_of(s, pos, n);
  }
  constexpr size_t find_last_of(const charT* s, size_t pos = npos) const {
    return view(*this).find_last_of(s, pos);
  }
  constexpr size_t find_last_of(charT c, size_t pos = npos) const {
    return view(*this).find_last_of(c, pos);
  }
  template <size_t M>
  constexpr size_t find_first_not_of(view str, size_t pos = 0) const noexcept {
    return view(*this).find_first_not_of(str, pos);
  }
  constexpr size_t find_first_not_of(const charT* s, size_t pos,
                                     size_t n) const {
    return view(*this).find_first_not_of(s, pos, n);
  }
  constexpr size_t find_first_not_of(const charT* s, size_t pos = 0) const {
    return view(*this).find_first_not_of(s, pos);
  }
  constexpr size_t find_first_not_of(charT c, size_t pos = 0) const {
    return view(*this).find_first_not_of(c, pos);
  }
  constexpr size_t find_last_not_of(view str, size_t pos = npos) const
      noexcept {
    return view(*this).find_last_not_of(str, pos);
  }
  constexpr size_t find_last_not_of(const charT* s, size_t pos,
                                    size_t n) const {
    return view(*this).find_last_not_of(s, pos, n);
  }
  constexpr size_t find_last_not_of(const charT* s, size_t pos = npos) const {
    return view(*this).find_last_not_of(s, pos);
  }
  constexpr size_t find_last_not_of(charT c, size_t pos = npos) const {
    return view(*this).find_last_not_of(c, pos);
  }

 private:
  charT data_[N + 1];  // exposition only
                       // (+1 is for terminating null)
};

}  // namespace experimental
}  // namespace std

#endif  // STD_EXPERIMENTAL_FIXED_STRING_H__
