/*
 * File:   NonNegativeInteger.hpp
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

/*
 * TODO:
 * - Binary operations
 */

#ifndef _NONNEGATIVEINTEGER_HPP
#define _NONNEGATIVEINTEGER_HPP

#define BOOST_DISABLE_ASSERTS

#include <iostream>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/integer_traits.hpp>
#include <boost/static_assert.hpp>
#include "detail/SimpleDigitVector.hpp"
#include "detail/lowlevel.hpp"

/*
namespace com {
namespace sputsoft {
namespace multiprecision {
*/

template <typename T, typename V=detail::SimpleDigitVector<T> >
class NonNegativeInteger {
  BOOST_STATIC_ASSERT(boost::integer_traits<T>::is_integer);
  BOOST_STATIC_ASSERT(!boost::integer_traits<T>::is_signed);
  template <typename S, typename W>
  friend std::ostream& operator<<(std::ostream& os, const NonNegativeInteger<S,W>& n);
private:
  boost::shared_ptr<V> digitvec;
  typedef typename V::size_type digitvec_size_type;
  static const unsigned int digitbits = boost::integer_traits<T>::digits;
  NonNegativeInteger(digitvec_size_type size, bool) : digitvec(new V(size)) {}
  T* digit_begin() { return digitvec->begin(); }
  T* digit_end() { return digitvec->end(); }
  const T* digit_begin() const { return digitvec->begin(); }
  const T* digit_end() const { return digitvec->end(); }
  NonNegativeInteger shift_left(size_t n) const;
  NonNegativeInteger shift_right(size_t n) const;
  NonNegativeInteger& shift_left_this(size_t n);
  NonNegativeInteger& shift_right_this(size_t n);
  static std::pair<NonNegativeInteger,NonNegativeInteger>
    long_divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v);
  static std::pair<NonNegativeInteger,NonNegativeInteger>
    simple_divide(const NonNegativeInteger<T,V>& u, T v);
public:
  static const NonNegativeInteger zero;
  NonNegativeInteger();
  NonNegativeInteger(T value);
  bool isZero() const;
  static NonNegativeInteger add(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v);
  static NonNegativeInteger subtract(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v);
  static NonNegativeInteger multiply(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v);
  static std::pair<NonNegativeInteger,NonNegativeInteger>
    divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v);
  static int compare(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v);
  NonNegativeInteger binary_shift(std::ptrdiff_t n) const;
  NonNegativeInteger& binary_shift_this(std::ptrdiff_t n);
  NonNegativeInteger& operator+=(const NonNegativeInteger<T,V>& v);
  NonNegativeInteger& operator-=(const NonNegativeInteger<T,V>& v);
  NonNegativeInteger& operator*=(const NonNegativeInteger<T,V>& v);
  NonNegativeInteger& operator/=(const NonNegativeInteger<T,V>& v);
  NonNegativeInteger& operator%=(const NonNegativeInteger<T,V>& v);
};

template <typename T, typename V> const NonNegativeInteger<T,V> NonNegativeInteger<T,V>::zero;

template <typename T, typename V>
NonNegativeInteger<T,V>::NonNegativeInteger() : digitvec(new V())
{}

template <typename T, typename V>
NonNegativeInteger<T,V>::NonNegativeInteger(T value) : digitvec(new V(1))
{
  if (value) {
    T* begin = digit_begin();
    *begin = value;
    digitvec->set_end(begin + 1);
  }
}

template <typename T, typename V>
inline bool NonNegativeInteger<T,V>::isZero() const
{
  return !digitvec->size();
}

/*
 * ADDITION
 */

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::add(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  NonNegativeInteger<T,V> r(std::max(u.digitvec->size(), v.digitvec->size())+1, true);
  T* rend = lowlevel::add_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator+=(const NonNegativeInteger<T,V>& x)
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
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::subtract(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (v.isZero()) return u;
  if (u.digitvec->size() < v.digitvec->size()) return zero;  // wrong usage
  NonNegativeInteger<T,V> r(u.digitvec->size(), true);
  T* rend = lowlevel::subtract(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator-=(const NonNegativeInteger<T,V>& v)
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
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::multiply(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (u.isZero() || v.isZero()) return zero;
  NonNegativeInteger<T,V> r(u.digitvec->size() + v.digitvec->size(), true);
  T* rend = lowlevel::multiply_sequences(u.digit_begin(), u.digit_end(), v.digit_begin(), v.digit_end(), r.digit_begin());
  r.digitvec->set_end(rend);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator*=(const NonNegativeInteger<T,V>& v)
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
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::simple_divide(const NonNegativeInteger<T,V>& u, T v)
{
  digitvec_size_type digits = u.digitvec->size();
  NonNegativeInteger<T,V> q(digits, true);
  const T *ufirst = u.digit_begin(), *ulast = u.digit_end();
  T* qlast = q.digit_begin() + digits - 1;
  T qh, rh;

  lowlevel::double_div((T) 0, *--ulast, v, qh, rh);

  if (qh)
    *qlast = qh;
  else
    --digits;
  while (ulast != ufirst) {
    lowlevel::double_div(rh, *--ulast, v, qh, rh);
    *--qlast = qh;
  }
  q.digitvec->set_end(q.digit_begin() + digits);

  return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(q, rh);
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
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::long_divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  unsigned int shift = 0;
  T vn1 = *(v.digit_end() - 1);
  while (!(vn1 & (((T) 1) << (digitbits - 1)))) { vn1 <<= 1; ++shift; }

  std::ptrdiff_t n = v.digitvec->size();
  std::ptrdiff_t m = u.digitvec->size() - n;

  const NonNegativeInteger<T,V> w = v.shift_left(shift);
  NonNegativeInteger<T,V> r(m+n+1, true);
  *(r.digit_begin() + m+n) = 0;
  lowlevel::shift_left(u.digit_begin(), u.digit_end(), r.digit_begin(), shift);

  NonNegativeInteger<T,V> q(m+1, true);

  const T* wfirst = w.digit_begin();
  const T* wlast = w.digit_end();
  T* rp = r.digit_begin();
  T* qp = q.digit_begin();
  T vn2 = *(wlast-2);

  T ujn, qh, k;
  for (int j=m; j >= 0; --j) {
    ujn = rp[j+n];
    qh = calc_qh(ujn, rp[j+n-1], rp[j+n-2], vn1, vn2);
    k = lowlevel::sequence_mult_digit_sub(rp+j, rp+j+n, wfirst, qh);
    rp[j+n] = ujn - k;
    if (k > ujn) {  // subtraction borrow?
      std::cout << "Add back" << std::endl;
      bool overflow;
      lowlevel::add_sequences_with_overflow(rp+j, rp+j+n+1, wfirst, wlast, rp+j, overflow);
      assert(overflow);
      --qh;
    }
    qp[j] = qh;
  }

  q.digitvec->set_end(qp[m] ? &qp[m+1] : &qp[m]);
  lowlevel::shift_right(rp, rp+n, rp, shift);
  while (n != 0 && !rp[n-1]) n--;
  r.digitvec->set_end(&rp[n]);

  return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(q, r);
}

template <typename T, typename V>
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (v.isZero() || compare(u, v) < 0)
    return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(zero, u);
  if (v.digitvec->size() == 1)
    return simple_divide(u, *v.digit_begin());
  return long_divide(u, v);
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator/=(const NonNegativeInteger<T,V>& v)
{
  return *this = divide(*this, v).first;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator%=(const NonNegativeInteger<T,V>& v)
{
  return *this = divide(*this, v).second;
}

/*
 * BINARY SHIFTING
 */

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::shift_left(size_t n) const
{
  if (!n) return *this;
  NonNegativeInteger<T,V> r(digitvec->size() + n/digitbits + 1, true);
  T* rlast = lowlevel::shift_left(digit_begin(), digit_end(), r.digit_begin(), n);
  r.digitvec->set_end(rlast);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::shift_left_this(size_t n)
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
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::shift_right(size_t n) const
{
  if (!n) return *this;
  size_t digitshift = n / digitbits;
  if (digitshift >= digitvec->size()) return zero;
  NonNegativeInteger<T,V> r(digitvec->size() - digitshift, true);
  T* rlast = lowlevel::shift_right(digit_begin(), digit_end(), r.digit_begin(), n);
  r.digitvec->set_end(rlast);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::shift_right_this(size_t n)
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
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::binary_shift(std::ptrdiff_t n) const
{
  return n >= 0 ? shift_left(n) : shift_right(-n);
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::binary_shift_this(std::ptrdiff_t n)
{
  return n >= 0 ? shift_left_this(n) : shift_right_this(-n);
}

/*
 * COMPARISONS
 */

template <typename T> int compare_values(const T& a, const T& b) {
  return a < b ? -1 : a > b ? 1 : 0;
}

template <typename T, typename V>
int NonNegativeInteger<T,V>::compare(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
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

template <typename T, typename V>
inline bool operator==(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) == 0;
}

template <typename T, typename V>
inline bool operator!=(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) != 0;
}

template <typename T, typename V>
inline bool operator<(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) < 0;
}

template <typename T, typename V>
inline bool operator<=(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) <= 0;
}

template <typename T, typename V>
inline bool operator>(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) > 0;
}

template <typename T, typename V>
inline bool operator>=(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) >= 0;
}

/*
 * Out-of-class operator overloads
 */

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator+(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::add(u, v);
}

template <typename T, typename V>
NonNegativeInteger<T,V> operator-(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::subtract(u, v);
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator*(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::multiply(u, v);
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator/(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::divide(u, v).first;
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator%(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::divide(u, v).second;
}

/*
 * Miscellaneous
 */

template <typename N> void output_number(std::ostream& os, N n) { os << n; }
template <> void output_number(std::ostream& os, unsigned char n) { os << (unsigned)n; }

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const NonNegativeInteger<T,V>& n)
{
  if (!n.isZero()) {
    const typename NonNegativeInteger<T,V>::digitvec_size_type bits =
                                                n.digitvec->size() * NonNegativeInteger<T,V>::digitbits;
    char buffer[(28*bits + 92)/93];  // 28/93 > ln2/ln10
    int digits=0;
    NonNegativeInteger<T,V> s = n, ten(10);
    while (!s.isZero()) {
      std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> > divrem = NonNegativeInteger<T,V>::divide(s, ten);
      s = divrem.first;
      buffer[digits++] = divrem.second.isZero() ? '0' : *divrem.second.digit_begin() + 48;
    }
    while (digits)
      os << buffer[--digits];
    if (!*(n.digit_end()-1)) {
      os << " | ";
      T const* p = n.digit_end();
      while (p != n.digit_begin()) {
        output_number(os, *--p);
        os << " ";
      }
      os << "(" << n.digitvec.use_count() << ")";
    }
  } else
    os << '0';

  return os;
}

/*
} // multiprecision
} // sputsoft
} // com
*/

#endif	/* _NONNEGATIVEINTEGER_HPP */
