/* 
 * File:   NaturalNumber.hpp
 * Author: jmr
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

#ifndef _NATURALNUMBER_HPP
#define	_NATURALNUMBER_HPP

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace com {
namespace sputsoft {
namespace multiprecision {

template <typename T> void swap(T& a, T& b) { T t = a;  a = b;  b = t; }

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

template <typename T>
class NaturalNumber {
  template <typename W>
  friend std::ostream& operator<<(std::ostream& os, const NaturalNumber<W>& n);
private:
  typedef SimpleLimbVector<T> V;
  boost::shared_ptr<V> limbvec;
  typedef T limb_type;
  typedef typename V::size_type limbvec_size_type;
  NaturalNumber(limbvec_size_type size, bool) : limbvec(new V(size)) {}
  limb_type* limb_begin() { return limbvec->begin(); }
  limb_type* limb_end() { return limbvec->end(); }
  const limb_type* limb_begin() const { return limbvec->begin(); }
  const limb_type* limb_end() const { return limbvec->end(); }
  NaturalNumber shift_left(size_t n) const;
  NaturalNumber shift_right(size_t n) const;
  static std::pair<NaturalNumber,NaturalNumber> simple_divide(const NaturalNumber<T>& u, limb_type v);
  static std::pair<NaturalNumber,NaturalNumber> long_divide(const NaturalNumber<T>& u, const NaturalNumber<T>& v);
public:
  NaturalNumber() : limbvec(new V()) {}
  NaturalNumber(limb_type value) : limbvec(new V(1)) {
    if (value) {
      *limb_begin() = value;
      limbvec->set_size(1);
    }
  }
  static const NaturalNumber zero;
  bool isZero() const { return !limbvec->size(); }
  static NaturalNumber add(const NaturalNumber<T>& u, const NaturalNumber<T>& v);
  NaturalNumber& operator+=(const NaturalNumber<T>& v);
  static NaturalNumber subtract(const NaturalNumber<T>& u, const NaturalNumber<T>& v);
  NaturalNumber& operator-=(const NaturalNumber<T>& v);
  static NaturalNumber multiply(const NaturalNumber<T>& u, const NaturalNumber<T>& v);
  NaturalNumber& operator*=(const NaturalNumber<T>& v);
  static std::pair<NaturalNumber,NaturalNumber> divide(const NaturalNumber<T>& u, const NaturalNumber<T>& v);
  int compare(const NaturalNumber<T>& v) const;
  NaturalNumber binary_shift(std::ptrdiff_t n) const;
};

template <typename T> const NaturalNumber<T> NaturalNumber<T>::zero;

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

template <typename T>
NaturalNumber<T> NaturalNumber<T>::add(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  if (u.isZero()) return v;
  if (v.isZero()) return u;
  NaturalNumber<T> r(std::max(u.limbvec->size(), v.limbvec->size())+1, true);
  limb_type* rbegin = r.limb_begin();
  limb_type* rend = adder(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T>
NaturalNumber<T>& NaturalNumber<T>::operator+=(const NaturalNumber<T>& x)
{
  if (isZero()) {
    *this = x;
  } else if (!x.isZero()) {
    if (limbvec.unique() && limbvec->capacity() > std::max(limbvec->size(), x.limbvec->size())) {
      limb_type* rbegin = limb_begin();
      limb_type* rend = adder(limb_begin(), limb_end(), x.limb_begin(), x.limb_end(), rbegin);
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

template <typename T>
NaturalNumber<T> NaturalNumber<T>::subtract(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  if (v.isZero()) return u;
  if (u.limbvec->size() < v.limbvec->size()) return zero;  // wrong usage
  NaturalNumber<T> r(u.limbvec->size(), true);
  limb_type* rbegin = r.limb_begin();
  limb_type* rend = subtracter(v.limb_begin(), v.limb_end(), u.limb_begin(), u.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T>
NaturalNumber<T>& NaturalNumber<T>::operator-=(const NaturalNumber<T>& v)
{
  if (!v.isZero() && limbvec->size() >= v.limbvec->size()) {
    if (limbvec.unique()) {
      limb_type* rbegin = limb_begin();
      limb_type* rend = subtracter(v.limb_begin(), v.limb_end(), rbegin, limb_end(), rbegin);
      limbvec->set_size(rend - rbegin);
    } else
      *this = subtract(*this, v);
  }
  return *this;
}

/*
 * MULTIPLICATION
 */

template <typename T>
T multaddadd(const T u, const T v, T& w, const T k)
{
  const unsigned int halfbits = boost::integer_traits<T>::digits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  T u1 = u >> halfbits, u0 = u & lowmask;
  T v1 = v >> halfbits, v0 = v & lowmask;
  T r = u0*v0 + (w & lowmask) + (k & lowmask);
  T s = u1*v0 + (w >> halfbits) + (r >> halfbits);
  T t = u0*v1 + (s & lowmask) + (k >> halfbits);
  w = (t << halfbits) | (r & lowmask);
  return u1*v1 + (s >> halfbits) + (t >> halfbits);
}

template <typename T>
T* multiplier(const T* ufirst, const T *ulast, const T* vfirst, const T *vlast, T* wfirst)
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

template <typename T>
NaturalNumber<T> NaturalNumber<T>::multiply(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  if (u.isZero() || v.isZero()) return zero;
  NaturalNumber<T> r(u.limbvec->size() + v.limbvec->size(), true);
  limb_type* rbegin = r.limb_begin();
  limb_type* rend = multiplier(u.limb_begin(), u.limb_end(), v.limb_begin(), v.limb_end(), rbegin);
  r.limbvec->set_size(rend - rbegin);
  return r;
}

template <typename T>
NaturalNumber<T>& NaturalNumber<T>::operator*=(const NaturalNumber<T>& v)
{
  return *this = multiply(*this, v);
}

/*
 * MULTIPLICATION
 */

template <typename T>
std::pair<NaturalNumber<T>, NaturalNumber<T> >
NaturalNumber<T>::simple_divide(const NaturalNumber<T>& u, limb_type v)
{
  const unsigned int halfbits = boost::integer_traits<limb_type>::digits / 2;
  const limb_type checkmask = ((limb_type) 1) << (halfbits - 1);
  const limb_type lowmask = (((limb_type) 1) << halfbits) - 1;
  unsigned int h = 0;

  while (!(v & checkmask)) { v <<= 1; ++h; }

  NaturalNumber<T> w = u.shift_left(h);
  limbvec_size_type limbs = w.limbvec->size();
  NaturalNumber<T> q(limbs, true);
  const limb_type *wfirst = w.limb_begin(), *wlast = w.limb_end() - 1;
  limb_type* qlast = q.limb_begin() + limbs - 1;
  limb_type r, t, s, z;

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

  return std::pair<NaturalNumber<T>, NaturalNumber<T> >(q, r >> h);
}

template <typename T>
std::pair<NaturalNumber<T>, NaturalNumber<T> >
NaturalNumber<T>::long_divide(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  return std::pair<NaturalNumber<T>, NaturalNumber<T> >(zero, zero);
}

template <typename T>
std::pair<NaturalNumber<T>, NaturalNumber<T> >
NaturalNumber<T>::divide(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  if (v.isZero() || u.compare(v) < 0)
    return std::pair<NaturalNumber<T>, NaturalNumber<T> >(zero, u);

  const unsigned int halfbits = boost::integer_traits<limb_type>::digits / 2;
  const limb_type highmask = ((limb_type) -1) << halfbits;
  if (v.limbvec->size() == 1 && !(*v.limb_begin() & highmask))
    return simple_divide(u, *v.limb_begin());
  else
    return long_divide(u, v);
}

/*
 * BINARY SHIFTING
 */

template <typename T>
NaturalNumber<T> NaturalNumber<T>::shift_left(size_t n) const
{
  if (!n) return *this;
  const unsigned int bits = boost::integer_traits<limb_type>::digits;
  const unsigned int shift = n % bits;
  n /= bits;
  NaturalNumber<T> r(limbvec->size() + n + 1, true);
  limb_type* rfirst = r.limb_begin(), *riter = rfirst;
  while (n--)
    *riter++ = 0;
  const limb_type *first = limb_begin(), *last = limb_end();
  if (shift != 0) {
    const unsigned int revshift = bits - shift;
    limb_type k = 0;
    while (first != last) {
      limb_type t = *first++;
      *riter++ = (t << shift) | k;
      k = t >> revshift;
    }
    if (k) *riter++ = k;
  } else {
    while (first != last)
      *riter++ = *first++;
  }
  r.limbvec->set_size(riter - rfirst);
  return r;
}

template <typename T>
NaturalNumber<T> NaturalNumber<T>::shift_right(size_t n) const
{
  if (!n) return *this;
  const unsigned int bits = boost::integer_traits<limb_type>::digits;
  const unsigned int shift = n % bits;
  n /= bits;
  if (n >= limbvec->size()) return zero;
  limbvec_size_type limbs = limbvec->size() - n;
  NaturalNumber<T> r(limbs, true);
  const limb_type *last = limb_end();
  limb_type *rfirst = r.limb_begin(), *rlast = rfirst + limbs;
  if (shift != 0) {
    const unsigned int revshift = bits - shift;
    limb_type t = *--last;
    limb_type k = t << revshift;
    --rlast;
    if (t >>= shift)
      *rlast = t;
    else
      --limbs;
    while (rlast != rfirst) {
      t = *--last;
      *--rlast = (t >> shift) | k;
      k = t << revshift;
    }
  } else {
    while (rlast != rfirst)
      *--rlast = *--last;
  }
  r.limbvec->set_size(limbs);
  return r;
}

template <typename T>
NaturalNumber<T> NaturalNumber<T>::binary_shift(std::ptrdiff_t n) const
{
  return n >= 0 ? shift_left(n) : shift_right(-n);
}

/*
 * COMPARISONS
 */

template <typename T>
int compare_values(const T& a, const T& b) {
  if (a < b) return -1;
  if (a > b) return  1;
  return 0;
}

template <typename T>
int NaturalNumber<T>::compare(const NaturalNumber<T>& v) const
{
  int c = compare_values(limbvec->size(), v.limbvec->size());
  if (c) return c;
  const limb_type *first1 = limb_begin(), *last1 = limb_end(), *last2 = v.limb_end();
  while (last1 != first1) {
    c = compare_values(*--last1, *--last2);
    if (c) return c;
  }
  return 0;
}

template <typename T>
inline bool operator==(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  return u.compare(v) == 0;
}

template <typename T>
inline bool operator!=(const NaturalNumber<T>& x, const NaturalNumber<T>& y)
{
  return x.compare(y) != 0;
}

template <typename T>
inline bool operator<(const NaturalNumber<T>& x, const NaturalNumber<T>& y)
{
  return x.compare(y) < 0;
}

template <typename T>
inline bool operator<=(const NaturalNumber<T>& x, const NaturalNumber<T>& y)
{
  return x.compare(y) <= 0;
}

template <typename T>
inline bool operator>(const NaturalNumber<T>& x, const NaturalNumber<T>& y)
{
  return x.compare(y) > 0;
}

template <typename T>
inline bool operator>=(const NaturalNumber<T>& x, const NaturalNumber<T>& y)
{
  return x.compare(y) >= 0;
}

/*
 * Out-of-class operator overloads
 */

template <typename T>
inline NaturalNumber<T> operator+(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  return NaturalNumber<T>::add(u, v);
}

template <typename T>
NaturalNumber<T> operator-(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  return NaturalNumber<T>::subtract(u, v);
}

template <typename T>
inline NaturalNumber<T> operator*(const NaturalNumber<T>& u, const NaturalNumber<T>& v)
{
  return NaturalNumber<T>::multiply(u, v);
}

/*
 * Miscellaneous
 */

template <typename T>
std::ostream& operator<<(std::ostream& os, const NaturalNumber<T>& n)
{
  if (!n.isZero()) {
    const int digits10 = boost::integer_traits<typename NaturalNumber<T>::limb_type>::digits10;
    char buffer[n.limbvec->size() * digits10];
    int digits=0;
    NaturalNumber<T> s = n, ten(10);
    while (!s.isZero()) {
      std::pair<NaturalNumber<T>, NaturalNumber<T> > divrem = NaturalNumber<T>::divide(s, ten);
      s = divrem.first;
      buffer[digits++] = divrem.second.isZero() ? '0' : *divrem.second.limb_begin() + 48;
    }
    while (digits)
      os << buffer[--digits];
    if (!n.limb_begin()[n.limbvec->size()-1]) {
      typename NaturalNumber<T>::limb_type const* p = n.limb_end();
      while (p != n.limb_begin())
        os << " " << *--p;
      os << " (" << n.limbvec.use_count() << ")";
    }
  } else
    os << '0';

  return os;
}

} // multiprecision
} // sputsoft
} // com

#endif	/* _NATURALNUMBER_HPP */

/*template <typename T>
T* multiplier(const T* u, const int m, const T* v, const int n, T* w)
{
  const int halfbits = boost::integer_traits<T>::digits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  const T highmask = lowmask << halfbits;
  T k, t, vj, ui, wij;
  for (int k=0; k < m; ++k)
    w[k] = 0;
  for (int j=0; j < 2*n; ++j) {
    k = 0;
    vj = (j%2 == 0) ? (v[j/2] & lowmask) : (v[j/2] >> halfbits);
    for (int i=0; i < 2*m; ++i) {
      ui = (i%2 == 0) ? (u[i/2] & lowmask) : (u[i/2] >> halfbits);
      wij = ((i+j)%2 == 0) ? (w[(i+j)/2] & lowmask) : (w[(i+j)/2] >> halfbits);
      t = ui*vj + wij + k;
      k = t >> halfbits;
      t &= lowmask;
      w[(i+j)/2] = ((i+j)%2 == 0) ? (w[(i+j)/2] & highmask) | t : (w[(i+j)/2] & lowmask) | (t << halfbits);
    }
    w[(j+2*m)/2] = ((j+2*m)%2 == 0) ? (w[(j+2*m)/2] & highmask) | k : (w[(j+2*m)/2] & lowmask) | (k << halfbits);
  }
  return w[m+n-1] == 0 ? &w[m+n-1] : &w[m+n];
}

/*template <typename T>
T* multiplier(const T* u, const int m, const T* v, const int n, T* w)
{
  const int halfbits = boost::integer_traits<T>::digits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  const T highmask = lowmask << halfbits;
  T ui, t, vj, wij;
  for (int i=0; i < m; ++i)
    w[i] = 0;
  for (int j=0; j < n; ++j) {
    t = 0;
    vj = v[j] & lowmask;
    for (int i=0; i < m; ++i) {
      ui = u[i];
      wij = w[i+j];
      t = (ui & lowmask)*vj + (wij & lowmask) + (t >> halfbits);
      wij = (wij & highmask) | (t & lowmask);
      t = (ui >> halfbits)*vj + (wij >> halfbits) + (t >> halfbits);
      w[i+j] = (wij & lowmask) | (t << halfbits);
    }
    w[j+m] = t >> halfbits;

    t = 0;
    vj = v[j] >> halfbits;
    wij = w[j];
    for (int i=0; i < m; ++i) {
      ui = u[i];
      t = (ui & lowmask)*vj + (wij >> halfbits) + (t >> halfbits);
      w[i+j] = (wij & lowmask) | (t << halfbits);
      wij = w[i+j+1];
      t = (ui >> halfbits)*vj + (wij & lowmask) + (t >> halfbits);
      wij = (wij & highmask) | (t & lowmask);
    }
    w[j+m] = wij | (t & highmask);
  }
  return w[m+n-1] == 0 ? &w[m+n-1] : &w[m+n];
}*/
