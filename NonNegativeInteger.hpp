/* 
 * File:   NonNegativeInteger.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created on 5. maj 2009, 14:43
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _NONNEGATIVEINTEGER_HPP
#define _NONNEGATIVEINTEGER_HPP

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace com {
namespace sputsoft {
namespace multiprecision {

template <typename T> void swap(T& a, T& b) { T t = a;  a = b;  b = t; }
template <typename T> int compare_values(const T& a, const T& b) {
  return (a < b) ? -1 : (a > b) ? 1 : 0;
}

template <typename T>
class SimpleLimbVector {
public:
  typedef T value_type;
  typedef size_t size_type;
  SimpleLimbVector() : _elems(new T[2]), _capacity(2), _size(0) {}
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
  static T multaddadd(const T u, const T v, T& w, const T k);
  static T* multiplier(const T* ufirst, const T *ulast, const T* vfirst, const T *vlast, T* wfirst);
  static T* shift_left_helper(const T* ufirst, const T* ulast, T* vfirst, size_t n);
  static T* shift_right_helper(const T* ufirst, const T* ulast, T* vfirst, size_t n);
  NonNegativeInteger shift_left(size_t n) const;
  NonNegativeInteger shift_right(size_t n) const;
  NonNegativeInteger& shift_left_self(size_t n);
  NonNegativeInteger& shift_right_self(size_t n);
  static bool long_divide_multsub_even(const T* vbegin, const T* vend, T* ubegin, const T q);
  static bool long_divide_multsub_odd(const T* vbegin, const T* vend, T* ubegin, const T q);
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
  NonNegativeInteger& binary_shift_self(std::ptrdiff_t n);
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

template <typename T>
T* adder(const T* first1, const T* last1, const T* first2, const T* last2, T* dst)
{
  T lx, ly, lz;
  bool carry = false;
  if (last1-first1 > last2-first2) {
    swap(first1, first2);
    swap(last1, last2);
  }
  while (first1 != last1) {
    lx = *first1;
    ly = *first2;
    if (carry) {
      lz = lx + ly + 1;
      carry = lz <= lx;
    } else {
      lz = lx + ly;
      carry = lz < lx;
    }
    *dst = lz;
    ++first1; ++first2; ++dst;
  }
  while (first2 != last2 && carry) {
    ly = *first2;
    lz = ly + 1;
    carry = lz <= ly;
    *dst = lz;
    ++first2; ++dst;
  }
  if (!carry) {
    while (first2 != last2) {
      *dst = *first2;
      ++first2; ++dst;
    }
  } else
    *dst++ = 1;
  return dst;
}

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::add(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  NonNegativeInteger<T,V> r(std::max(u.limbvec->size(), v.limbvec->size())+1, true);
  T* rbegin = r.limb_begin();
  T* rend = adder(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
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
      T* rend = adder(limb_begin(), limb_end(), x.limb_begin(), x.limb_end(), rbegin);
      limbvec->set_size(rend - rbegin);
    } else
      *this = add(*this, x);
  }
  return *this;
}

/*
 * SUBTRACTION
 */

// Requires last1-first1 <= last2-first2
template <typename T>
T* subtracter(const T* first1, const T* last1, const T* first2, const T* last2, T* dst)
{
  T* pr = dst;
  T lx, ly, lz;
  bool borrow = false;
  while (first1 != last1) {
    lx = *first2;
    ly = *first1;
    if (borrow) {
      lz = lx - ly - 1;
      borrow = lx <= ly;
    } else {
      lz = lx - ly;
      borrow = lx < ly;
    }
    *pr = lz;
    ++first1; ++first2; ++pr;
  }
  while (first2 != last2 && borrow) {
    lx = *first2;
    borrow = lx == 0;
    *pr = lx - 1;
    ++first2; ++pr;
  }
  while (first2 != last2) {
    *pr = *first2;
    ++first2; ++pr;
  }
  while (pr != dst && *(pr-1) == 0)
    --pr;
  return pr;
}

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::subtract(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (v.isZero()) return u;
  if (u.limbvec->size() < v.limbvec->size()) return zero;  // wrong usage
  NonNegativeInteger<T,V> r(u.limbvec->size(), true);
  T* rbegin = r.limb_begin();
  T* rend = subtracter(v.limb_begin(), v.limb_end(), u.limb_begin(), u.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::operator-=(const NonNegativeInteger<T,V>& v)
{
  if (!v.isZero() && limbvec->size() >= v.limbvec->size()) {
    if (limbvec.unique()) {
      T* rbegin = limb_begin();
      T* rend = subtracter(v.limb_begin(), v.limb_end(), rbegin, limb_end(), rbegin);
      limbvec->set_size(rend - rbegin);
    } else
      *this = subtract(*this, v);
  }
  return *this;
}

/*
 * MULTIPLICATION
 */

template <typename T, typename V>
T NonNegativeInteger<T,V>::multaddadd(const T u, const T v, T& w, const T k)
{
  T u1 = u >> halfbits, u0 = u & lowmask;
  T v1 = v >> halfbits, v0 = v & lowmask;
  T r = u0*v0 + (w & lowmask) + (k & lowmask);
  T s = u1*v0 + (w >> halfbits) + (r >> halfbits);
  T t = u0*v1 + (s & lowmask) + (k >> halfbits);
  w = (t << halfbits) | (r & lowmask);
  return u1*v1 + (s >> halfbits) + (t >> halfbits);
}

template <typename T, typename V>
T* NonNegativeInteger<T,V>::multiplier(const T* ufirst, const T *ulast, const T* vfirst, const T *vlast, T* wfirst)
{
  const T *ui;
  T *wij;
  std::fill(wfirst, wfirst + (ulast - ufirst), 0);
  while (vfirst != vlast) {
    T k = 0;
    ui = ufirst;
    wij = wfirst;
    while (ui != ulast)
      k = multaddadd(*ui++, *vfirst, *wij++, k);
    *wij = k;
    ++vfirst;
    ++wfirst;
  }
  wij = wfirst + (ulast - ufirst);
  if (!*(wij-1)) --wij;
  return wij;
}

template <typename T, typename V>
NonNegativeInteger<T,V> NonNegativeInteger<T,V>::multiply(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (u.isZero() || v.isZero()) return zero;
  NonNegativeInteger<T,V> r(u.limbvec->size() + v.limbvec->size(), true);
  T* rbegin = r.limb_begin();
  T* rend = multiplier(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
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
  const T checkmask = ((T) 1) << (halfbits - 1);
  unsigned int h = 0;

  while (!(v & checkmask)) { v <<= 1; ++h; }

  NonNegativeInteger<T,V> w = u.shift_left(h);
  limbvec_size_type limbs = w.limbvec->size();
  NonNegativeInteger<T,V> q(limbs, true);
  const T *wfirst = w.limb_begin(), *wlast = w.limb_end() - 1;
  T* qlast = q.limb_begin() + limbs - 1;
  T r, t, s, z;

  s = *wlast;
  t = s >> halfbits;
  z = (t%v << halfbits) | (s & lowmask);
  t = (t/v << halfbits) | (z/v);
  if (t)
    *qlast = t;
  else
    --limbs;
  r = z%v;
  while (wlast != wfirst) {
    s = *--wlast;
    t = (r << halfbits) | (s >> halfbits);
    z = (t%v << halfbits) | (s & lowmask);
    *--qlast = (t/v << halfbits) | (z/v);
    r = z%v;
  }
  q.limbvec->set_size(limbs);

  return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(q, r >> h);
}

template <typename T>
T gethalf(const T* p, std::ptrdiff_t i)
{
  const int halfbits = boost::integer_traits<T>::digits/2;
  return (i%2) ? (p[i/2] >> halfbits) : (p[i/2] & ((((T) 1) << halfbits) - 1));
}

template <typename T>
void sethalf(T* p, std::ptrdiff_t i, T v)
{
  const int halfbits = boost::integer_traits<T>::digits/2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  const T highmask = lowmask << halfbits;
  if (i%2)
    p[i/2] = (p[i/2] & lowmask) | (v << halfbits);
  else
    p[i/2] = (p[i/2] & highmask) | (v & lowmask);
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
  while (vbegin != vend) {
    v = *vbegin++;
    t = (u >> halfbits) - q*(v & lowmask) + k;
    u = (u & lowmask) | (t << halfbits);
    k = t >> halfbits;
    if (k) k |= highmask;
    *ubegin = u;
    u = *++ubegin;
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
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::long_divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  T vn1, vn2, s, qh, rh;
  unsigned int h = 0;

  s = *(v.limb_end() - 1);
  const T checkmask = ((T) 1) << (limbbits - 1);
  while (!(s & checkmask)) { s <<= 1; ++h; }

  NonNegativeInteger<T,V> w = v.shift_left(h);
  NonNegativeInteger<T,V> r = u.shift_left(h);

  std::ptrdiff_t n = 2*w.limbvec->size();
  std::ptrdiff_t m = 2*u.limbvec->size() - n;
  NonNegativeInteger<T,V> q(m/2+1, true);

  const T* wp = w.limb_begin();
  T* rp = r.limb_begin();
  T* qp = q.limb_begin();

  vn1 = s >> halfbits;
  vn2 = s & lowmask;

  if (r.limbvec->size() == u.limbvec->size())
    sethalf(rp, m+n, (T) 0);

  for (int j=m; j >= 0; --j) {
    s = (gethalf(rp, j+n) << halfbits) | gethalf(rp, j+n-1);
    qh = s / vn1;
    rh = s % vn1;
    while ((qh & highmask) || (qh*vn2 > ((rh << halfbits) | gethalf(rp, j+n-2)))) {
      --qh;
      rh += vn1;
    }
    if (j%2 == 0) {
      long_divide_multsub_even(wp, wp+n/2, rp, qh);
    } else {
      long_divide_multsub_odd(wp, wp+n/2, rp, qh);
    }
    sethalf(qp, j, qh);
  }
  q.limbvec->set_size(qp[m/2] ? m/2+1 : m/2);
  while (n != 0 && !rp[n-1]) n--;
  r.limbvec->set_size(n);

  return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(q, r.shift_right(h));
}

template <typename T, typename V>
std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >
NonNegativeInteger<T,V>::divide(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  if (v.isZero() || compare(u, v) < 0)
    return std::pair<NonNegativeInteger<T,V>, NonNegativeInteger<T,V> >(zero, u);

  if (v.limbvec->size() == 1 && !(*v.limb_begin() & highmask))
    return simple_divide(u, *v.limb_begin());
  else
    return long_divide(u, v);
}

/*
 * BINARY SHIFTING
 */

template <typename T, typename V>
T* NonNegativeInteger<T,V>::shift_left_helper(const T* ufirst, const T* ulast, T* vfirst, size_t n)
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
  T* rlast = shift_left_helper(limb_begin(), limb_end(), rfirst, n);
  r.limbvec->set_size(rlast - rfirst);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::shift_left_self(size_t n)
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
T* NonNegativeInteger<T,V>::shift_right_helper(const T* ufirst, const T* ulast, T* vfirst, size_t n)
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
  T* rlast = shift_right_helper(limb_begin(), limb_end(), rfirst, n);
  r.limbvec->set_size(rlast - rfirst);
  return r;
}

template <typename T, typename V>
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::shift_right_self(size_t n)
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
NonNegativeInteger<T,V>& NonNegativeInteger<T,V>::binary_shift_self(std::ptrdiff_t n)
{
  return n >= 0 ? shift_left_self(n) : shift_right_self(-n);
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
