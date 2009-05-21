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
 * - Self shifts
 * - Inner loops without conditionals
 * - Division on full limbs faster?
 * - Binary operations
 */

#ifndef _NONNEGATIVEINTEGER_HPP
#define _NONNEGATIVEINTEGER_HPP

#include <iostream>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/integer_traits.hpp>
#include <boost/static_assert.hpp>
#include "lowlevel.hpp"

namespace com {
namespace sputsoft {
namespace multiprecision {

template <typename T> int compare_values(const T& a, const T& b) {
  return (a < b) ? -1 : (a > b) ? 1 : 0;
}

template <typename T>
class SimpleLimbVector {
public:
  typedef T value_type;
  typedef size_t size_type;
  SimpleLimbVector() : _capacity(2), _size(0), _elems(new T[2]) {}
  explicit SimpleLimbVector(size_type __size)
    : _capacity(std::max(__size, (size_type) 2)), _size(0), _elems(new T[_capacity]) {}
  size_type size() const { return _size; }
  size_type capacity() const { return _capacity; }
  void set_size(size_type __size) { _size = __size; }
  T* begin() { return _elems; }
  T* end() { return _elems + _size; }
  const T* begin() const { return _elems; }
  const T* end() const { return _elems + _size; }
private:
  size_type _capacity;
  size_type _size;
  T* _elems;
};

template <typename T, typename V=SimpleLimbVector<T> >
class NonNegativeInteger {
  BOOST_STATIC_ASSERT(boost::integer_traits<T>::is_integer);
  BOOST_STATIC_ASSERT(!boost::integer_traits<T>::is_signed);
  template <typename S, typename W>
  friend std::ostream& operator<<(std::ostream& os, const NonNegativeInteger<S,W>& n);
private:
  boost::shared_ptr<V> limbvec;
  typedef typename V::size_type limbvec_size_type;
  static const unsigned int limbbits = boost::integer_traits<T>::digits;
  static const unsigned int halfbits = limbbits / 2;
  static const T lowmask = (((T) 1) << halfbits) - 1;
  static const T highmask = lowmask << halfbits;
  NonNegativeInteger(limbvec_size_type size, bool) : limbvec(new V(size)) {}
  T* limb_begin() { return limbvec->begin(); }
  T* limb_end() { return limbvec->end(); }
  const T* limb_begin() const { return limbvec->begin(); }
  const T* limb_end() const { return limbvec->end(); }
  static T* left_shifter(const T* ufirst, const T* ulast, T* vfirst, size_t n);
  static T* right_shifter(const T* ufirst, const T* ulast, T* vfirst, std::ptrdiff_t n);
  NonNegativeInteger shift_left(size_t n) const;
  NonNegativeInteger shift_right(size_t n) const;
  NonNegativeInteger& shift_left_this(size_t n);
  NonNegativeInteger& shift_right_this(size_t n);
  static bool long_divide_multsub_even(const T* vbegin, const T* vend, T* ubegin, const T q);
  static bool long_divide_multsub_odd(const T* vbegin, const T* vend, T* ubegin, const T q);
  static void long_divide_addback_even(const T* vbegin, const T* vend, T* ubegin);
  static void long_divide_addback_odd(const T* vbegin, const T* vend, T* ubegin);
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
};

template <typename T, typename V> const NonNegativeInteger<T,V> NonNegativeInteger<T,V>::zero;

template <typename T, typename V>
NonNegativeInteger<T,V>::NonNegativeInteger() : limbvec(new V())
{}

template <typename T, typename V>
NonNegativeInteger<T,V>::NonNegativeInteger(T value) : limbvec(new V(1))
{
  if (value) {
    *limb_begin() = value;
    limbvec->set_size(1);
  }
}

template <typename T, typename V>
inline bool NonNegativeInteger<T,V>::isZero() const
{
  return !limbvec->size();
}

/*
 * ADDITION
 */

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::add(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  NonNegativeInteger<T,V> r(std::max(u.limbvec->size(), v.limbvec->size())+1, true);
  T* rbegin = r.limb_begin();
  T* rend = lowlevel::add_sequences(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator+=(const NonNegativeInteger<T,V>& x)
{
  if (isZero()) {
    *this = x;
  } else if (!x.isZero()) {
    if (limbvec.unique() && limbvec->capacity() > std::max(limbvec->size(), x.limbvec->size())) {
      T* rbegin = limb_begin();
      T* rend = lowlevel::add_sequences(limb_begin(), limb_end(), x.limb_begin(), x.limb_end(), rbegin);
      limbvec->set_size(rend - rbegin);
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
  if (u.limbvec->size() < v.limbvec->size()) return zero;  // wrong usage
  NonNegativeInteger<T,V> r(u.limbvec->size(), true);
  T* rbegin = r.limb_begin();
  T* rend = lowlevel::subtract(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator-=(const NonNegativeInteger<T,V>& v)
{
  if (!v.isZero() && limbvec->size() >= v.limbvec->size()) {
    if (limbvec.unique()) {
      T* begin = limb_begin();
      T* end = subtracter(begin, limb_end(), v.limb_begin(), v.limb_end(), begin);
      limbvec->set_size(end - begin);
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
  NonNegativeInteger<T,V> r(u.limbvec->size() + v.limbvec->size(), true);
  T* rbegin = r.limb_begin();
  T* rend = lowlevel::multiply_sequences(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
  //T* rend = multiplier(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator*=(const NonNegativeInteger<T,V>& v)
{
  return *this = multiply(*this, v);
}

/*
 * DIVISION
 */

template <typename T, typename V>
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::simple_divide(const NonNegativeInteger<T,V>& u, T v)
{
  limbvec_size_type limbs = u.limbvec->size();
  NonNegativeInteger<T,V> q(limbs, true);
  const T *ufirst = u.limb_begin(), *ulast = u.limb_end();
  T* qlast = q.limb_begin() + limbs - 1;

  std::pair<T, T> quotrem = lowlevel::double_div((T) 0, *--ulast, v);

  if (quotrem.first)
    *qlast = quotrem.first;
  else
    --limbs;
  while (ulast != ufirst) {
    quotrem = lowlevel::double_div(quotrem.second, *--ulast, v);
    *--qlast = quotrem.first;
  }
  q.limbvec->set_size(limbs);

  return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(q, quotrem.second);
}

template <typename T, typename V>
bool NonNegativeInteger<T,V>::long_divide_multsub_even(const T* vbegin, const T* vend, T* ubegin, const T q)
{
  T k = 0, t, u, v;
  while (vbegin != vend) {
    u = *ubegin;
    v = *vbegin++;
    t = (u & lowmask) - q*(v & lowmask) + k;
    u = (u & highmask) | (t & lowmask);
    k = t >> halfbits;
    if (k) k |= highmask;
    t = (u >> halfbits) - q*(v >> halfbits) + k;
    u = (u & lowmask) | (t << halfbits);
    k = t >> halfbits;
    if (k) k |= highmask;
    *ubegin++ = u;
  }
  u = *ubegin;
  t = (u & lowmask) + k;
  *ubegin = (u & highmask) | (t & lowmask);
  k = t >> halfbits;
  return k;
}

template <typename T, typename V>
bool NonNegativeInteger<T,V>::long_divide_multsub_odd(const T* vbegin, const T* vend, T* ubegin, const T q)
{
  T k = 0, t, u, v;
  u = *ubegin;
  for (; vbegin != vend; ++vbegin) {
    v = *vbegin;
    t = (u >> halfbits) - q*(v & lowmask) + k;
    u = (u & lowmask) | (t << halfbits);
    k = t >> halfbits;
    if (k) k |= highmask;
    *ubegin = u;
    ++ubegin;
    u = *ubegin;
    t = (u & lowmask) - q*(v >> halfbits) + k;
    u = (u & highmask) | (t & lowmask);
    k = t >> halfbits;
    if (k) k |= highmask;
  }
  t = (u >> halfbits) + k;
  *ubegin = (u & lowmask) | (t << halfbits);
  k = t >> halfbits;
  return k;
}

template <typename T, typename V>
void NonNegativeInteger<T,V>::long_divide_addback_even(const T* vbegin, const T* vend, T* ubegin)
{
  T u, v, t;
  bool carry = false;
  std::cout << "long_divide_addback_even" << std::endl;
  for (; vbegin != vend; ++vbegin, ++ubegin) {
    u = *ubegin;
    v = *vbegin;
    if (carry) {
      t = u + v + 1;
      carry = t <= u;
    } else {
      t = u + v;
      carry = t < u;
    }
    *ubegin = t;
  }
  assert(carry);
  u = *ubegin;
  t = (u & lowmask) + 1;
  assert(t & highmask);
  *ubegin = (u & highmask) | (t & lowmask);
}

template <typename T, typename V>
void NonNegativeInteger<T,V>::long_divide_addback_odd(const T* vbegin, const T* vend, T* ubegin)
{
  T u, v, v1, v2, t;
  std::cout << "long_divide_addback_odd" << std::endl;
  u = *ubegin;
  v1 = *vbegin;
  v = v1 << halfbits;
  t = u + v;
  *ubegin = t;
  bool carry = t < u;
  for (++vbegin, ++ubegin; vbegin != vend; ++vbegin, ++ubegin) {
    u = *ubegin;
    v2 = v1;
    v1 = *vbegin;
    v = (v2 >> halfbits) | (v1 << halfbits);
    if (carry) {
      t = u + v + 1;
      carry = t <= u;
    } else {
      t = u + v;
      carry = t < u;
    }
    *ubegin = t;
  }
  assert(carry);
  u = *ubegin;
  v = v1 >> halfbits;
  t = u + v + 1;
  assert(t <= u);
  *ubegin = t;
}

template <typename T, typename V>
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::long_divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  unsigned int shift = 0;
  T s = *(v.limb_end() - 1);
  while (!(s & (((T) 1) << (limbbits - 1)))) { s <<= 1; ++shift; }
  T vn1 = s >> halfbits;
  T vn2 = s & lowmask;

  std::ptrdiff_t n = v.limbvec->size();
  std::ptrdiff_t m = u.limbvec->size() - n;

  const NonNegativeInteger<T,V> w = v.shift_left(shift);
  NonNegativeInteger<T,V> r(m+n+1, true);
  *(r.limb_begin() + m+n) = 0;
  left_shifter(u.limb_begin(), u.limb_end(), r.limb_begin(), shift);

  NonNegativeInteger<T,V> q(m+1, true);

  const T* wfirst = w.limb_begin();
  const T* wlast = w.limb_end();
  T* rp = r.limb_begin();
  T* qp = q.limb_begin();

  T qh1, qh2, rh, t;
  for (int j=m; j >= 0; --j) {
    s = rp[j+n];
    t = rp[j+n-1] >> halfbits;
    qh1 = s / vn1;
    rh  = s % vn1;
    while ((qh1 & highmask) || (qh1*vn2 > ((rh << halfbits) | t))) {
      --qh1;
      rh += vn1;
    }
    if (long_divide_multsub_odd(wfirst, wlast, rp + j, qh1)) {
      long_divide_addback_odd(wfirst, wlast, rp + j);
      --qh1;
    }
    t = rp[j+n-1];
    s = (rp[j+n] << halfbits) | (t >> halfbits);
    t &= lowmask;
    qh2 = s / vn1;
    rh  = s % vn1;
    while ((qh2 & highmask) || (qh2*vn2 > ((rh << halfbits) | t))) {
      --qh2;
      rh += vn1;
    }
    if (long_divide_multsub_even(wfirst, wlast, rp + j, qh2)) {
      long_divide_addback_even(wfirst, wlast, rp + j);
      --qh2;
    }
    qp[j] = (qh1 << halfbits) | qh2;
  }
  q.limbvec->set_size(qp[m] ? m+1 : m);
  r.shift_right_this(shift);
  while (n != 0 && !rp[n-1]) n--;
  r.limbvec->set_size(n);

  return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(q, r);
}

template <typename T, typename V>
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (v.isZero() || compare(u, v) < 0)
    return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(zero, u);
  if (v.limbvec->size() == 1)
    return simple_divide(u, *v.limb_begin());
  return long_divide(u, v);
}

/*
 * BINARY SHIFTING
 */

template <typename T, typename V>
T* NonNegativeInteger<T,V>::left_shifter(const T* ufirst, const T* ulast, T* vfirst, size_t n)
{
  const unsigned int shift = n % limbbits;
  n /= limbbits;
  while (n--)
    *vfirst++ = 0;
  if (shift != 0) {
    const unsigned int revshift = limbbits - shift;
    T k = 0;
    while (ufirst != ulast) {
      T t = *ufirst++;
      *vfirst++ = (t << shift) | k;
      k = t >> revshift;
    }
    if (k) *vfirst++ = k;
  } else {
    while (ufirst != ulast)
      *vfirst++ = *ufirst++;
  }
  return vfirst;
}

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::shift_left(size_t n) const
{
  if (!n) return *this;
  NonNegativeInteger<T,V> r(limbvec->size() + n/limbbits + 1, true);
  T* rfirst = r.limb_begin();
  T* rlast = left_shifter(limb_begin(), limb_end(), rfirst, n);
  r.limbvec->set_size(rlast - rfirst);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::shift_left_this(size_t n)
{
  if (n) {
    /*if (limbvec.unique() && limbvec->capacity() >= limbvec->size() + n/limbbits + 1) {
      T* first = limb_begin();
      T* last = shift_left_helper(first, last, first, n);
      limbvec->set_size(last - first);
    } else*/
      *this = shift_left(n);
  }
  return *this;
}

template <typename T, typename V>
T* NonNegativeInteger<T,V>::right_shifter(const T* ufirst, const T* ulast, T* vfirst, std::ptrdiff_t n)
{
  const unsigned int shift = n % limbbits;
  n /= limbbits;
  if (n >= ulast - ufirst) return vfirst;
  limbvec_size_type limbs = ulast - ufirst - n;
  T *vlast = vfirst + limbs;
  if (shift != 0) {
    const unsigned int revshift = limbbits - shift;
    T t = *--ulast;
    T k = t << revshift;
    --vlast;
    if (t >>= shift)
      *vlast = t;
    else
      --limbs;
    while (vlast != vfirst) {
      t = *--ulast;
      *--vlast = (t >> shift) | k;
      k = t << revshift;
    }
  } else {
    while (vlast != vfirst)
      *--vlast = *--ulast;
  }
  return vfirst + limbs;
}

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::shift_right(size_t n) const
{
  if (!n) return *this;
  size_t limbshift = n / limbbits;
  if (limbshift >= limbvec->size()) return zero;
  NonNegativeInteger<T,V> r(limbvec->size() - limbshift, true);
  T* rfirst = r.limb_begin();
  T* rlast = right_shifter(limb_begin(), limb_end(), rfirst, n);
  r.limbvec->set_size(rlast - rfirst);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::shift_right_this(size_t n)
{
  if (n) {
    /*if (limbvec.unique()) {
      T* first = limb_begin();
      T* last = shift_left_helper(first, last, first, n);
      r.limbvec->set_size(last - first);
    } else*/
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

template <typename T, typename V>
int NonNegativeInteger<T,V>::compare(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  int c = compare_values(u.limbvec->size(), v.limbvec->size());
  if (c) return c;
  const T *first1 = u.limb_begin(), *last1 = u.limb_end(), *last2 = v.limb_end();
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

/*
 * Miscellaneous
 */

template <typename N> void output_number(std::ostream& os, N n) { os << n; }
template <> void output_number(std::ostream& os, unsigned char n) { os << (unsigned)n; }

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const NonNegativeInteger<T,V>& n)
{
  if (!n.isZero()) {
    const typename NonNegativeInteger<T,V>::limbvec_size_type bits =
                                                n.limbvec->size() * NonNegativeInteger<T,V>::limbbits;
    char buffer[(28*bits + 92)/93];  // 28/93 > ln2/ln10
    int digits=0;
    NonNegativeInteger<T,V> s = n, ten(10);
    while (!s.isZero()) {
      std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> > divrem = NonNegativeInteger<T,V>::divide(s, ten);
      s = divrem.first;
      buffer[digits++] = divrem.second.isZero() ? '0' : *divrem.second.limb_begin() + 48;
    }
    while (digits)
      os << buffer[--digits];
    if (!*(n.limb_end()-1)) {
      os << " | ";
      T const* p = n.limb_end();
      while (p != n.limb_begin()) {
        output_number(os, *--p);
        os << " ";
      }
      os << "(" << n.limbvec.use_count() << ")";
    }
  } else
    os << '0';

  return os;
}

} // multiprecision
} // sputsoft
} // com

#endif	/* _NONNEGATIVEINTEGER_HPP */
