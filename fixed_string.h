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
#include <iostream>
#include <initializer_list>
#include <iterator>

namespace std {
namespace experimental {

template<class charT, size_t N>
class basic_fixed_string;

template<class charT, size_t N>
constexpr basic_fixed_string<charT, N>
    make_fixed_string(const charT(&arr)[N]);

template<class charT,  size_t N, size_t M>
constexpr basic_fixed_string<charT, N+M>
operator+(const basic_fixed_string<charT, N>& lhs,
     const basic_fixed_string<charT, M>& rhs) noexcept;

template<class charT, size_t N1, size_t M>
constexpr basic_fixed_string<charT, N1-1+M>
operator+(const charT(&lhs)[N1],
      const basic_fixed_string<charT, M>& rhs) noexcept;

template<class charT, size_t N>
constexpr basic_fixed_string<charT, N+1>
operator+(charT lhs,
      const basic_fixed_string<charT,N>& rhs) noexcept;

template<class charT, size_t N, size_t M1>
constexpr basic_fixed_string<charT, N+M1-1>
operator+(const basic_fixed_string<charT, N>& lhs,
          const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N>
constexpr basic_fixed_string<charT, N+1>
operator+(const basic_fixed_string<charT, N>& lhs,
          charT rhs) noexcept;

template<class charT, size_t N, size_t M>
constexpr bool operator==(
	 const basic_fixed_string<charT, N>& lhs,
	 const basic_fixed_string<charT, M>& rhs)
	 noexcept;

template<class charT, size_t N1, size_t M>
constexpr bool operator==(const charT (&lhs)[N1],
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N, size_t M1>
constexpr bool operator==(
	const basic_fixed_string<charT, N>& lhs,
	const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N, size_t M>
constexpr bool operator!=(
	const basic_fixed_string<charT, N>& lhs,
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N1, size_t M>
constexpr bool operator!=(const charT (&lhs)[N1],
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N, size_t M1>
constexpr bool operator!=(
	const basic_fixed_string<charT, N>& lhs,
	const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N, size_t M>
constexpr bool operator<(
	const basic_fixed_string<charT, N>& lhs,
	const basic_fixed_string<charT, M>& rhs)
    noexcept;

template<class charT, size_t N1, size_t M>
constexpr bool operator<(const charT (&lhs)[N1],
    const basic_fixed_string<charT, M>& rhs)
    noexcept;

template<class charT, size_t N, size_t M1>
constexpr bool operator<(
	const basic_fixed_string<charT, N>& lhs,
	const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N, size_t M>
constexpr bool operator>(
	const basic_fixed_string<charT, N>& lhs,
	const basic_fixed_string<charT, M>& rhs)
    noexcept;

template<class charT, size_t N1, size_t M>
constexpr bool operator>(const charT (&lhs)[N1],
    const basic_fixed_string<charT, M>& rhs)
    noexcept;

template<class charT, size_t N, size_t M1>
constexpr bool operator>(
	const basic_fixed_string<charT, N>& lhs,
	const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N, size_t M>
constexpr bool operator<=(
	const basic_fixed_string<charT, N>& lhs,
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N1, size_t M>
constexpr bool operator<=(const charT (&lhs)[N1],
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N, size_t M1>
constexpr bool operator<=(
	const basic_fixed_string<charT, N>& lhs,
	const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N, size_t M>
constexpr bool operator>=(
	const basic_fixed_string<charT, N>& lhs,
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N1, size_t M>
constexpr bool operator>=(const charT (&lhs)[N1],
	const basic_fixed_string<charT, M>& rhs)
	noexcept;

template<class charT, size_t N, size_t M1>
constexpr bool operator>=(
	const basic_fixed_string<charT, N>& lhs,
	const charT (&rhs)[M1]) noexcept;

template<class charT, size_t N>
constexpr void swap(
	basic_fixed_string<charT, N>& lhs,
	basic_fixed_string<charT, N>& rhs) noexcept;

template<class charT, size_t N>
    basic_ostream<charT>& operator<<(
    basic_ostream<charT>& os,
    const basic_fixed_string<charT, N>& str);

template<size_t N> using fixed_string
	= basic_fixed_string<char, N>;
template<size_t N> using u16fixed_string
	= basic_fixed_string<char16_t, N>;
template<size_t N> using u32fixed_string
	= basic_fixed_string<char32_t, N>;
template<size_t N> using wfixed_string
	= basic_fixed_string<wchar_t, N>;

template<size_t N>
constexpr int stoi(const fixed_string<N>& str,
                   size_t* idx = 0, int base = 10);
template<size_t N>
constexpr long stol(const fixed_string<N>& str,
                    size_t* idx = 0, int base = 10);
template<size_t N>
constexpr unsigned long stoul(const fixed_string<N>& str,
                          size_t* idx = 0, int base = 10);
template<size_t N>
constexpr long long stoll(const fixed_string<N>& str,
                          size_t* idx = 0, int base = 10);
template<size_t N>
constexpr unsigned long long stoull(
	const fixed_string<N>& str,
	size_t* idx = 0, int base = 10);

template<size_t N>
constexpr float stof(
	const fixed_string& str, size_t* idx = 0);
template<size_t N>
constexpr double stod(
	const fixed_string& str, size_t* idx = 0);
template<size_t N>
constexpr long double stold(
	const fixed_string& str, size_t* idx = 0);

constexpr size_t __count_num_digits_signed(long long val);
constexpr size_t __count_num_digits_unsigned(unsigned long long val);

template<int val>
constexpr fixed_string<__count_num_digits_signed(val)>
    to_fixed_string_i;

template<unsigned val>
constexpr fixed_string<__count_num_digits_unsigned(val)>
    to_fixed_string_u;

template<long val>
constexpr fixed_string<__count_num_digits_signed(val)>
    to_fixed_string_l;

template<unsigned long val>
constexpr fixed_string<__count_num_digits_unsigned(val)>
    to_fixed_string_ul;

template<long long val>
constexpr fixed_string<__count_num_digits_signed(val)>
    to_fixed_string_ll;

template<unsigned long long val>
constexpr fixed_string<__count_num_digits_unsigned(val)>
    to_fixed_string_ull;

template<size_t N>
constexpr int stoi(const wfixed_string<N>& str,
                   size_t* idx = 0, int base = 10);
template<size_t N>
constexpr long stol(const wfixed_string<N>& str,
                    size_t* idx = 0, int base = 10);
template<size_t N>
constexpr unsigned long stoul(
	const wfixed_string<N>& str,
	size_t* idx = 0, int base = 10);

template<size_t N>
constexpr long long stoll(
	const wfixed_string<N>& str, size_t* idx = 0,
	int base = 10);

template<size_t N>
constexpr unsigned long long stoull(
    	const wfixed_string<N>& str,
        size_t* idx = 0, int base = 10);

template<size_t N>
constexpr float stof(
	    const wfixed_string<N>& str, size_t* idx = 0);

template<size_t N>
constexpr double stod(const wfixed_string<N>& str,
                      size_t* idx = 0);

template<size_t N>
constexpr long double stold(
	const wfixed_string<N>& str,
    size_t* idx = 0);

template<int val>
constexpr wfixed_string<__count_num_digits_signed(val)>
    to_wfixed_string_i;

template<unsigned val>
constexpr wfixed_string<__count_num_digits_unsigned(val)>
    to_wfixed_string_u;

template<long val>
constexpr wfixed_string<__count_num_digits_signed(val)>
    to_wfixed_string_l;

template<unsigned long val>
constexpr wfixed_string<__count_num_digits_unsigned(val)>
    to_wfixed_string_ul;

template<long long val>
constexpr wfixed_string<__count_num_digits_signed(val)>
    to_wfixed_string_ll;

template<unsigned long long val>
constexpr wfixed_string<__count_num_digits_unsigned(val)>
    to_wfixed_string_ull;

template<class charT, size_t N>
class basic_fixed_string
{
public:
	typedef charT value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;

	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef reverse_iterator<iterator>
	    reverse_iterator;
	typedef reverse_iterator<const_iterator>
	    const_reverse_iterator;
	static constexpr size_t npos = -1;

	constexpr basic_fixed_string() noexcept;
	constexpr basic_fixed_string
	    (const basic_fixed_string& str) noexcept;

	constexpr basic_fixed_string& operator=
		(const basic_fixed_string& str) noexcept;
	constexpr basic_fixed_string& operator=
		(const charT (&s)[N+1]);
	constexpr basic_fixed_string& operator=
		(charT c);
	constexpr basic_fixed_string& operator=
		(initializer_list<charT>);

	constexpr iterator begin() noexcept;
	constexpr const_iterator begin() const noexcept;
	constexpr iterator end() noexcept;
	constexpr const_iterator end() const noexcept;

	constexpr reverse_iterator rbegin() noexcept;
	constexpr const_reverse_iterator rbegin()
	    const noexcept;
	constexpr reverse_iterator rend() noexcept;
	constexpr const_reverse_iterator rend()
	    const noexcept;

	constexpr const_iterator cbegin()
	    const noexcept;
	constexpr const_iterator cend() const noexcept;
	constexpr const_reverse_iterator crbegin()
	    const noexcept;
	constexpr const_reverse_iterator crend()
	    const noexcept;

	constexpr size_t size() const noexcept;
	constexpr size_t length() const noexcept;

	constexpr bool empty() const noexcept;

	constexpr const_reference operator[](size_t pos)
	    const noexcept;

	constexpr reference operator[](size_t pos) noexcept;
	constexpr const_reference at(size_t n) const;
	constexpr reference at(size_t n);

	constexpr const charT& front() const;
	constexpr charT& front();
	constexpr const charT& back() const;
	constexpr charT& back();

	constexpr basic_fixed_string& assign(
		const basic_fixed_string& str);
	constexpr basic_fixed_string& assign(
		const charT(&s)[N]);

	constexpr template<class InputIterator>
	basic_string& assign(InputIterator first,
	                     InputIterator last);
	    // last - first must == N

	constexpr basic_fixed_string& assign(
		initializer_list<charT>);
	    // initializer_list length must == N

	template <size_t M>
	constexpr basic_fixed_string& replace(size_t pos1,
			size_t n1,
	    const basic_fixed_string<charT, M>& str);

	template <size_t M>
	constexpr basic_fixed_string& replace(
		size_t pos1, size_t n1,
		const basic_fixed_string<charT,M>& str,
		size_t pos2, size_t n2 = npos);

	constexpr basic_fixed_string& replace(
		size_t pos, size_t n1,
		const charT* s, size_t n2);

	constexpr basic_fixed_string& replace(size_t pos,
	    size_t n1, const charT* s);

	constexpr basic_fixed_string& replace(size_t pos,
	    size_t n1, size_t n2, charT c);

	template <size_t M>
	constexpr basic_fixed_string& replace(
		const_iterator i1, const_iterator i2,
		const basic_fixed_string<charT, M>& str);

	constexpr basic_fixed_string& replace(
		const_iterator i1, const_iterator i2,
		const charT* s, size_t n);

	constexpr basic_fixed_string& replace(
		const_iterator i1, const_iterator i2,
		const charT* s);

	constexpr basic_fixed_string& replace(
		const_iterator i1, const_iterator i2,
		size_t n, charT c);

	template<class InputIterator>
	constexpr basic_fixed_string& replace(
		const_iterator i1, const_iterator i2,
		InputIterator j1, InputIterator j2);

	constexpr basic_string& replace(
		const_iterator, const_iterator,
		initializer_list<charT>);

	constexpr size_t copy(charT* s,
			size_t n, size_t pos = 0) const;

	constexpr void swap(basic_fixed_string& str);

	constexpr const charT* c_str() const noexcept;
	constexpr const charT* data() const noexcept;

	template<size_t M>
	constexpr size_t find (
		const basic_fixed_string<charT,M>& str,
		size_t pos = 0) const noexcept;

	constexpr size_t find (
		const charT* s, size_t pos, size_t n) const;

	constexpr size_t find (
		const charT* s, size_t pos = 0) const;

	constexpr size_t find (charT c, size_t pos = 0) const;

	template<size_t M>
	constexpr size_t rfind(
		const basic_fixed_string<charT,M>& str,
		size_t pos = npos) const noexcept;

	constexpr size_t rfind(
		const charT* s, size_t pos, size_t n) const;

	constexpr size_t rfind(
		const charT* s, size_t pos = npos) const;

	constexpr size_t rfind(
		charT c, size_t pos = npos) const;

	template<size_t M>
	constexpr size_t find_first_of(
		const basic_fixed_string<charT,M>& str,
		size_t pos = 0) const noexcept;

	constexpr size_t find_first_of(
		const charT* s, size_t pos, size_t n) const;

	constexpr size_t find_first_of(
		const charT* s, size_t pos = 0) const;

	constexpr size_t find_first_of(
		charT c, size_t pos = 0) const;

	template<size_t M>
	constexpr size_t find_last_of (
		const basic_fixed_string<charT,M>& str,
		size_t pos = npos) const noexcept;

	constexpr size_t find_last_of (
		const charT* s, size_t pos, size_t n) const;

	constexpr size_t find_last_of (
		const charT* s, size_t pos = npos) const;

	constexpr size_t find_last_of (
		charT c, size_t pos = npos) const;

	template<size_t M>
	constexpr size_t find_first_not_of(
		const basic_fixed_string<charT,M>& str,
		size_t pos = 0) const noexcept;

	constexpr size_t find_first_not_of(
		const charT* s, size_t pos, size_t n) const;

	constexpr size_t find_first_not_of(const charT* s,
	    size_t pos = 0) const;
	constexpr size_t find_first_not_of(
		charT c, size_t pos = 0) const;

	template<size_t M>
	constexpr size_t find_last_not_of (
		const basic_fixed_string<charT,M>& str,
		size_t pos = npos) const noexcept;

	constexpr size_t find_last_not_of (
		const charT* s, size_t pos, size_t n) const;

	constexpr size_t find_last_not_of (
		const charT* s, size_t pos = npos) const;
	constexpr size_t find_last_not_of (
		charT c, size_t pos = npos) const;

	template<size_t M> constexpr
	basic_fixed_string<charT, M>
	substr(size_t pos = 0) const;

	template<size_t M> constexpr int compare(
		const basic_fixed_string<charT, M>& str)
	    const noexcept;

	template<size_t M> constexpr int compare(
		size_t pos1, size_t n1,
		const basic_fixed_string<charT, M>& str)
	    const;

	template<size_t M> constexpr int compare(
		size_t pos1, size_t n1,
		const basic_fixed_string<charT, M>& str,
		size_t pos2, size_t n2 = npos) const;

	constexpr int compare(const charT* s) const;
	constexpr int compare(size_t pos1, size_t n1,
	                      const charT* s) const;
	constexpr int compare(size_t pos1, size_t n1,
	                      const charT* s, size_t n2) const;

private:
	charT data_[N+1]; // exposition only
	                  // (+1 is for terminating null)
};

}  // namespace experimental
}  // namespace std

#endif  // STD_EXPERIMENTAL_FIXED_STRING_H__
