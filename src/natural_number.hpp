/*
 * File:   natural_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-05 12:43Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _NATURAL_NUMBER_HPP
#define _NATURAL_NUMBER_HPP

#ifdef NDEBUG
#define BOOST_DISABLE_ASSERTS
#endif

#include <iostream>
#include <iomanip>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/integer_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/random.hpp>
#include <detail/digit_vector.hpp>
#include <detail/lowlevel.hpp>


namespace sputsoft {
namespace multiprecision {


class DivideByZero {};

template <typename T, typename V=detail::digit_vector<T> >
class natural_number {
  BOOST_STATIC_ASSERT(boost::integer_traits<T>::is_integer);
  BOOST_STATIC_ASSERT(!boost::integer_traits<T>::is_signed);
  template <typename S, typename W>
  friend void show_internal(std::ostream& os, const natural_number<S,W>& n);
private:
  boost::shared_ptr<V> digitvec;
  typedef typename V::size_type digitvec_size_type;
  static const unsigned int digitbits = boost::integer_traits<T>::digits;
  natural_number(digitvec_size_type size, bool) : digitvec(new V(size)) {}
  T* digit_begin() { return digitvec->begin(); }
  T* digit_end() { return digitvec->end(); }
  const T* digit_begin() const { return digitvec->begin(); }
  const T* digit_end() const { return digitvec->end(); }
  natural_number shift_left(size_t n) const;
  natural_number shift_right(size_t n) const;
  natural_number& shift_left_this(size_t n);
  natural_number& shift_right_this(size_t n);
  static std::pair<natural_number, natural_number>
    divide_long(const natural_number<T,V>& u, const natural_number<T,V>& v);
public:
  typedef T digit_type;
  static const natural_number zero;
  natural_number();
  natural_number(T value);
  bool isZero() const;
  static natural_number add(const natural_number<T,V>& u, const natural_number<T,V>& v);
  static natural_number subtract(const natural_number<T,V>& u, const natural_number<T,V>& v);
  static natural_number multiply(const natural_number<T,V>& u, const natural_number<T,V>& v);
  static std::pair<natural_number, T> divide_simple(const natural_number<T,V>& u, T v);
  static std::pair<natural_number,natural_number>
    divide(const natural_number<T,V>& u, const natural_number<T,V>& v);
  static int compare(const natural_number<T,V>& u, const natural_number<T,V>& v);
  natural_number binary_shift(std::ptrdiff_t n) const;
  natural_number& binary_shift_this(std::ptrdiff_t n);
  static T binary_and(const natural_number<T,V>& u, const T v);
  static natural_number binary_and(const natural_number<T,V>& u, const natural_number<T,V>& v);
  static natural_number binary_or(const natural_number<T,V>& u, const natural_number<T,V>& v);
  static natural_number binary_xor(const natural_number<T,V>& u, const natural_number<T,V>& v);
  template <typename Generator>
  static natural_number make_random(Generator& generator, std::size_t bits);
  size_t lg_floor() const;
  size_t lg_ceil() const;
  natural_number& operator+=(const natural_number<T,V>& v);
  natural_number& operator-=(const natural_number<T,V>& v);
  natural_number& operator*=(const natural_number<T,V>& v);
  natural_number& operator/=(const natural_number<T,V>& v);
  natural_number& operator%=(const natural_number<T,V>& v);
  natural_number& operator&=(const natural_number<T,V>& v);
  natural_number& operator|=(const natural_number<T,V>& v);
  natural_number& operator^=(const natural_number<T,V>& v);
  void swap(natural_number& v);
};

template <typename T, typename V>
const natural_number<T,V> natural_number<T,V>::zero;

template <typename T, typename V>
natural_number<T,V>::natural_number() : digitvec(new V())
{}

template <typename T, typename V>
natural_number<T,V>::natural_number(T value) : digitvec(new V(1))
{
  if (value) {
    T* begin = digit_begin();
    *begin = value;
    digitvec->set_end(begin + 1);
  }
}

template <typename T, typename V>
inline bool natural_number<T,V>::isZero() const
{
  return !digitvec->size();
}

/*
 * ADDITION
 */

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::add(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  natural_number<T,V> r(std::max(u.digitvec->size(), v.digitvec->size())+1, true);
  T* rend = lowlevel::add_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator+=(const natural_number<T,V>& x)
{
  if (isZero()) {
    *this = x;
  } else if (!x.isZero()) {
    if (digitvec.unique() && digitvec->request_size(std::max(digitvec->size(), x.digitvec->size()))) {
      T* rend = lowlevel::add_sequences(digit_begin(), digit_end(), x.digit_begin(), x.digit_end(), digit_begin());
      digitvec->set_end(rend);
    } else
      *this = add(*this, x);
  }
  return *this;
}

/*
 * SUBTRACTION
 */

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::subtract(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (v.isZero()) return u;
  if (u.digitvec->size() < v.digitvec->size()) return zero;  // wrong usage
  natural_number<T,V> r(u.digitvec->size(), true);
  T* rend = lowlevel::subtract(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator-=(const natural_number<T,V>& v)
{
  if (!v.isZero() && digitvec->size() >= v.digitvec->size()) {
    if (digitvec.unique()) {
      T* begin = digit_begin();
      T* end = lowlevel::subtract(begin, digit_end(), v.digit_begin(), v.digit_end(), begin);
      digitvec->set_end(end);
    } else
      *this = subtract(*this, v);
  }
  return *this;
}

/*
 * MULTIPLICATION
 */

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::multiply(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (u.isZero() || v.isZero()) return zero;
  natural_number<T,V> r(u.digitvec->size() + v.digitvec->size(), true);
  T* rend = lowlevel::multiply_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator*=(const natural_number<T,V>& v)
{
  if (!isZero()) {
    if (v.isZero()) {
      *this = zero;
    } else if (v.digitvec->size() == 1 && digitvec.unique() && digitvec->request_size(digitvec->size() + 1)) {
      T* first = digit_begin();
      T* last = digit_end();
      T k = lowlevel::multiply_sequence_with_digit(first, last, first, *v.digit_begin());
      if (k) {
        *last++ = k;
        digitvec->set_end(last);
      }
    } else
      *this = multiply(*this, v);
  }
  return *this;
}

/*
 * DIVISION
 */

template <typename T, typename V>
std::pair<natural_number<T,V>, T>
natural_number<T,V>::divide_simple(const natural_number<T,V>& u, T v)
{
  if (!v)
    throw DivideByZero();
  if (u.isZero())
    return std::pair<natural_number<T,V>, T>(zero, (T) 0);
  if (u.digitvec->size() == 1 && *u.digit_begin() < v)
    return std::pair<natural_number<T,V>, T>(zero, *u.digit_begin());

  const T *ufirst = u.digit_begin(), *ulast = u.digit_end();
  digitvec_size_type digits = u.digitvec->size();
  natural_number<T,V> q(digits, true);
  T *qlast = q.digit_begin() + digits, *qiter = qlast-1;
  T qh, rh;

  lowlevel::double_div((T) 0, *--ulast, v, qh, rh);

  if (qh)
    *qiter = qh;
  else
    --qlast;
  while (ulast != ufirst) {
    lowlevel::double_div(rh, *--ulast, v, qh, rh);
    *--qiter = qh;
  }
  q.digitvec->set_end(qlast);

  return std::pair<natural_number<T,V>, T>(q, rh);
}

template <typename T>
inline T calc_qh(T ujn, T ujn1, T ujn2, T vn1, T vn2)
{
  T qh, rh, high, low;

  if (ujn == vn1) {
    qh = (T) -1;
    rh = ujn1 + vn1;
    if (rh < vn1) return qh;
  } else
    lowlevel::double_div(ujn, ujn1, vn1, qh, rh);

  lowlevel::double_mult(qh, vn2, low, high);
  while (high > rh || (high == rh && low > ujn2)) {
    --qh;
    rh += vn1;
    if (rh < vn1) break;  // overflow
    lowlevel::double_mult(qh, vn2, low, high);
  }

  return qh;
}

template <typename T, typename V>
std::pair<natural_number<T,V>, natural_number<T,V> >
natural_number<T,V>::divide_long(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (v.isZero())
    throw DivideByZero();
  if (compare(u, v) < 0)
    return std::pair<natural_number<T,V>, natural_number<T,V> >(zero, u);

  unsigned int shift = digitbits-1 - lowlevel::lg_floor(*(v.digit_end() - 1));
  const natural_number<T,V> w = v.shift_left(shift);
  std::ptrdiff_t n = v.digitvec->size();
  std::ptrdiff_t m = u.digitvec->size() - n;
  natural_number<T,V> r(m+n+1, true);
  natural_number<T,V> q(m+1, true);
  const T* wfirst = w.digit_begin();
  const T* wlast = w.digit_end();
  T* rp = r.digit_begin();
  T* qp = q.digit_begin();
  T vn1 = *(wlast-1);
  T vn2 = *(wlast-2);
  T ujn, qh, k;

  *(r.digit_begin() + m+n) = 0;
  lowlevel::shift_left(u.digit_begin(), u.digit_end(), r.digit_begin(), shift);

  for (int j=m; j >= 0; --j) {
    ujn = rp[j+n];
    qh = calc_qh(ujn, rp[j+n-1], rp[j+n-2], vn1, vn2);
    k = lowlevel::sequence_mult_digit_sub(rp+j, rp+j+n, wfirst, qh);
    rp[j+n] = ujn - k;
    if (k > ujn) {  // subtraction borrow?
      bool overflow;
      lowlevel::add_sequences_with_overflow(rp+j, rp+j+n+1, wfirst, wlast, rp+j, overflow);
      assert(overflow);
      --qh;
    }
    qp[j] = qh;
  }

  q.digitvec->set_end(qp[m] ? &qp[m+1] : &qp[m]);
  lowlevel::shift_right(rp, rp+n+1, rp, shift);  // is not n digits long until AFTER the shift
  while (n != 0 && !rp[n-1]) n--;
  r.digitvec->set_end(&rp[n]);

  return std::pair<natural_number<T,V>, natural_number<T,V> >(q, r);
}

template <typename T, typename V>
std::pair<natural_number<T,V>, natural_number<T,V> >
natural_number<T,V>::divide(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (v.digitvec->size() == 1)
    return divide_simple(u, *v.digit_begin());
  return divide_long(u, v);
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator/=(const natural_number<T,V>& v)
{
  return *this = divide(*this, v).first;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator%=(const natural_number<T,V>& v)
{
  return *this = divide(*this, v).second;
}

/*
 * BINARY OPERATIONS
 */

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::shift_left(size_t n) const
{
  if (!n) return *this;
  natural_number<T,V> r(digitvec->size() + n/digitbits + 1, true);
  T* rlast = lowlevel::shift_left(digit_begin(), digit_end(), r.digit_begin(), n);
  r.digitvec->set_end(rlast);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::shift_left_this(size_t n)
{
  if (n && !isZero()) {
    if (digitvec.unique() && digitvec->request_size(digitvec->size() + n/digitbits + 1)) {
      T* first = digit_begin();
      T* last = lowlevel::shift_left(first, digit_end(), first, n);
      digitvec->set_end(last);
    } else
      *this = shift_left(n);
  }
  return *this;
}

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::shift_right(size_t n) const
{
  if (!n) return *this;
  size_t digitshift = n / digitbits;
  if (digitshift >= digitvec->size()) return zero;
  natural_number<T,V> r(digitvec->size() - digitshift, true);
  T* rlast = lowlevel::shift_right(digit_begin(), digit_end(), r.digit_begin(), n);
  r.digitvec->set_end(rlast);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::shift_right_this(size_t n)
{
  if (n) {
    if (digitvec.unique()) {
      T* first = digit_begin();
      T* last = lowlevel::shift_right(first, digit_end(), first, n);
      digitvec->set_end(last);
    } else
      *this = shift_right(n);
  }
  return *this;
}

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::binary_shift(std::ptrdiff_t n) const
{
  return n >= 0 ? shift_left(n) : shift_right(-n);
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::binary_shift_this(std::ptrdiff_t n)
{
  return n >= 0 ? shift_left_this(n) : shift_right_this(-n);
}

template <typename T, typename V>
T natural_number<T,V>::binary_and(const natural_number<T,V>& u, const T v)
{
  return u.isZero() ? T(0) : *u.digit_begin() & v;
}

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::binary_and(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (u.isZero() || v.isZero()) return zero;
  natural_number<T,V> r(std::min(u.digitvec->size(), v.digitvec->size()), true);
  T* rend = lowlevel::and_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator&=(const natural_number<T,V>& v)
{
  if (!isZero()) {
    if (!v.isZero()) {
      if (digitvec.unique()) {
        T* end = lowlevel::and_sequences(digit_begin(), digit_end(), v.digit_begin(), v.digit_end(), digit_begin());
        digitvec->set_end(end);
      } else
        *this = binary_and(*this, v);
    } else
      *this = zero;
  }
  return *this;
}

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::binary_or(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  natural_number<T,V> r(std::max(u.digitvec->size(), v.digitvec->size()), true);
  T* rend = lowlevel::or_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator|=(const natural_number<T,V>& v)
{
  if (!v.isZero()) {
    if (digitvec.unique() && digitvec->request_size(std::max(digitvec->size(), v.digitvec->size()))) {
      T* end = lowlevel::or_sequences(digit_begin(), digit_end(), v.digit_begin(), v.digit_end(), digit_begin());
      digitvec->set_end(end);
    } else
      *this = binary_or(*this, v);
  }
  return *this;
}

template <typename T, typename V>
natural_number<T,V> natural_number<T,V>::binary_xor(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  natural_number<T,V> r(std::max(u.digitvec->size(), v.digitvec->size()), true);
  T* rend = lowlevel::xor_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
natural_number<T,V>& natural_number<T,V>::operator^=(const natural_number<T,V>& v)
{
  if (!v.isZero()) {
    if (digitvec.unique() && digitvec->request_size(std::max(digitvec->size(), v.digitvec->size()))) {
      T* end = lowlevel::xor_sequences(digit_begin(), digit_end(), v.digit_begin(), v.digit_end(), digit_begin());
      digitvec->set_end(end);
    } else
      *this = binary_xor(*this, v);
  }
  return *this;
}

template <typename T, typename V>
size_t natural_number<T,V>::lg_floor() const
{
  return isZero() ? -1 : (lowlevel::lg_floor(*(digit_end() - 1)) + (digitvec->size() - 1) * digitbits);
}

template <typename T, typename V>
size_t natural_number<T,V>::lg_ceil() const
{
  if (isZero()) return -1;
  const T* first = digit_begin();
  const T* last = digit_end();
  T t = *--last;
  unsigned s = lowlevel::lg_floor(t);
  size_t r = s + (digitvec->size() - 1) * digitbits;

  if (t != ((T) 1) << s) return r+1;
  while (last != first)
    if (*--last) return r+1;
  return r;
}

/*
 * COMPARISONS
 */

template <typename T> int compare_values(const T& a, const T& b) {
  return a < b ? -1 : a > b ? 1 : 0;
}

template <typename T, typename V>
int natural_number<T,V>::compare(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  int c = compare_values(u.digitvec->size(), v.digitvec->size());
  if (c) return c;
  const T *first1 = u.digit_begin(), *last1 = u.digit_end(), *last2 = v.digit_end();
  while (last1 != first1) {
    c = compare_values(*--last1, *--last2);
    if (c) return c;
  }
  return 0;
}

/*
 * Miscellaneous
 */

template <typename T, typename V>
template <typename Generator>
natural_number<T,V> natural_number<T,V>::make_random(Generator& generator, std::size_t bits)
{
  if (!bits) return zero;

  natural_number<T,V> r((bits+digitbits-1) / digitbits, true);
  T *rbegin = r.digit_begin(), *riter = rbegin;

  boost::uniform_int<T> digit_dist(0, boost::integer_traits<T>::const_max);
  boost::variate_generator<Generator&, boost::uniform_int<T> > digit_uni(generator, digit_dist);

  while (bits >= digitbits) {
    *riter++ = digit_uni();
    bits -= digitbits;
  }

  if (bits) {
    boost::uniform_int<T> rem_dist(0, (T(1) << bits) - 1);
    boost::variate_generator<Generator&, boost::uniform_int<T> > rem_uni(generator, rem_dist);
    *riter++ = rem_uni();
  }

  while (riter != rbegin && !*(riter-1))
    --riter;
  r.digitvec->set_end(riter);

  return r;
}

template <typename T, typename V>
void show_internal(std::ostream& os, const natural_number<T,V>& n)
{
  os << n;
  os << " | ";
  const T* p = n.digit_end();
  while (p != n.digit_begin()) {
    output_number(os, *--p);
    os << " ";
  }
  os << "(" << n.digitvec.use_count() << ")";
}


/*
 * EXTERNAL FUNCTIONS
 */

template <typename T, typename V>
inline bool operator==(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::compare(u, v) == 0;
}

template <typename T, typename V>
inline bool operator!=(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::compare(u, v) != 0;
}

template <typename T, typename V>
inline bool operator<(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::compare(u, v) < 0;
}

template <typename T, typename V>
inline bool operator<=(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::compare(u, v) <= 0;
}

template <typename T, typename V>
inline bool operator>(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::compare(u, v) > 0;
}

template <typename T, typename V>
inline bool operator>=(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::compare(u, v) >= 0;
}

template <typename T, typename V>
inline natural_number<T,V> operator+(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::add(u, v);
}

template <typename T, typename V>
natural_number<T,V> operator-(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::subtract(u, v);
}

template <typename T, typename V>
inline natural_number<T,V> operator*(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::multiply(u, v);
}

template <typename T, typename V>
inline natural_number<T,V> operator/(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::divide(u, v).first;
}

template <typename T, typename V>
inline natural_number<T,V> operator%(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::divide(u, v).second;
}

template <typename T, typename V>
inline natural_number<T,V> operator&(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::binary_and(u, v);
}

template <typename T, typename V>
inline T operator&(const natural_number<T,V>& u, const T v)
{
  return natural_number<T,V>::binary_and(u, v);
}

template <typename T, typename V>
inline natural_number<T,V> operator|(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::binary_or(u, v);
}

template <typename T, typename V>
inline natural_number<T,V> operator^(const natural_number<T,V>& u, const natural_number<T,V>& v)
{
  return natural_number<T,V>::binary_xor(u, v);
}

template <typename T, typename V>
void natural_number<T,V>::swap(natural_number<T,V>& v)
{
  digitvec.swap(v.digitvec);
}

template <typename T, typename V>
void swap(natural_number<T,V>& u, natural_number<T,V>& v)
{
  u.swap(v);
}

template <typename N> inline void output_number(std::ostream& os, N n) { os << n; }
template <> inline void output_number(std::ostream& os, unsigned char n) { os << (unsigned)n; }

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const natural_number<T,V>& n)
{
  if (!n.isZero()) {
    const unsigned base_log10 = boost::integer_traits<T>::digits10;
    T buffer[28*(n.lg_floor()+1)/(93*base_log10)+1];  // 28/93 > ln2/ln10
    T denom = 1;
    for (unsigned u=0; u < base_log10; u++) denom *= 10;
    natural_number<T,V> s = n;
    int k=0;
    while (!s.isZero()) {
      std::pair<natural_number<T,V>, T> divrem = natural_number<T,V>::divide_simple(s, denom);
      s = divrem.first;
      buffer[k++] = divrem.second;
    }
    output_number(os, buffer[--k]);
    while (k) {
      os << std::setw(base_log10) << std::setfill('0');
      output_number(os, buffer[--k]);
    }
  } else
    os << '0';

  return os;
}


} // multiprecision
} // sputsoft


#endif	/* _NATURAL_NUMBER_HPP */
