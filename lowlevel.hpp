/*
 * File:   lowlevel.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-19 14:39Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _LOWLEVEL_HPP
#define _LOWLEVEL_HPP

#include <boost/integer_traits.hpp>

//#if defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )

namespace com {
namespace sputsoft {
namespace multiprecision {
namespace lowlevel {

template <typename T>
T* reset_sequence(T* first, T* last)
{
  while (first != last)
    *first++ = 0;
  return first;
}

/*
 * ADDITION
 */

// Assumes ulast-ufirst > vlast-vfirst
template <typename T>
inline std::pair<bool, T*> add_sequences_with_overflow(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst)
{
  T lx, ly, lz;
  bool carry = false;
  for (; vfirst != vlast; ++vfirst, ++ufirst, ++dst) {
    lx = *vfirst;
    ly = *ufirst;
    if (carry) {
      lz = lx + ly + 1;
      carry = lz <= lx;
    } else {
      lz = lx + ly;
      carry = lz < lx;
    }
    *dst = lz;
  }
  for (; ufirst != ulast && carry; ++ufirst, ++dst) {
    lz = *ufirst + 1;
    *dst = lz;
    carry = !lz;
  }
  if (ufirst != ulast)
    dst = std::copy(ufirst, ulast, dst);
  return std::make_pair(carry, dst);
}

template <typename T>
T* add_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst)
{
  std::pair<bool, T*> ret = (ulast - ufirst > vlast - vfirst) ?
          add_sequences_with_overflow(ufirst, ulast, vfirst, vlast, dst)
        : add_sequences_with_overflow(vfirst, vlast, ufirst, ulast, dst);
  dst = ret.second;
  if (ret.first) *dst++ = 1;
  return dst;
}


/*
 * SUBTRACTION
 */

// Requires last1-first1 >= last2-first2
template <typename T>
T* subtract(const T* first1, const T* last1, const T* first2, const T* last2, T* dst)
{
  T* pr = dst;
  T lx, ly, lz;
  bool borrow = false;
  while (first2 != last2) {
    lx = *first1;
    ly = *first2;
    if (borrow) {
      lz = lx - ly - 1;
      borrow = lx <= ly;
    } else {
      lz = lx - ly;
      borrow = lx < ly;
    }
    *pr = lz;
    ++first2; ++first1; ++pr;
  }
  while (first1 != last1 && borrow) {
    lx = *first1;
    borrow = lx == 0;
    *pr = lx - 1;
    ++first1; ++pr;
  }
  while (first1 != last1) {
    *pr = *first1;
    ++first1; ++pr;
  }
  while (pr != dst && *(pr-1) == 0)
    --pr;
  return pr;
}


/*
 * MULTIPLICATION
 */

template <typename T>
inline std::pair<T, T> double_mult(const T u, const T v)
{
  const unsigned int halfbits = boost::integer_traits<T>::digits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  T u1 = u >> halfbits, u0 = u & lowmask;
  T v1 = v >> halfbits, v0 = v & lowmask;
  T r = u0*v0;
  T s = u1*v0 + (r >> halfbits);
  T t = u0*v1 + (s & lowmask);
  return std::make_pair(u1*v1 + (s >> halfbits) + (t >> halfbits),
                        (t << halfbits) | (r & lowmask));
}

template <typename T>
inline T mult_add_add(T& w, const T u, const T v, const T k)
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
inline T multiply_sequence_with_limb(const T* first, const T* last, T* dst, T v)
{
  T k = 0;
  while (first != last)
    k = mult_add_add(*dst++, *first++, v, k);
  return k;
}

// Assumes ufirst != ulast and vfirst != vlast
template <typename T>
T* multiply_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* wfirst)
{
  T *wlast=reset_sequence(wfirst, wfirst + (ulast - ufirst));
  while (vfirst != vlast)
    *wlast++ = multiply_sequence_with_limb(ufirst, ulast, wfirst++, *vfirst++);
  if (!*(wlast-1)) --wlast;
  return wlast;
}

template <typename T>
inline T mult_sub_sub(T& w, const T u, const T v, T k)
{
  T wt = -w;
  k = mult_add_add(wt, u, v, k);
  w = -wt;
  return k;
}

template <typename T>
T sequence_mult_limb_sub(T* ufirst, T* ulast, const T* vfirst, T q)
{
  T k = 0;
  while (ufirst != ulast)
    k = mult_sub_sub(*ufirst++, *vfirst++, q, k);
  return k;
}


/*
 * DIVISION
 */

template <typename T>
std::pair<T,T> double_div(const T uhigh, const T ulow, const T v)
{
  const unsigned int limbbits = boost::integer_traits<T>::digits;
  const unsigned int halfbits = limbbits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  const T highmask = lowmask << halfbits;

  const T v1 = v >> halfbits;
  const T u1 = ulow >> halfbits;
  const T u0 = ulow & lowmask;
  T q1, q0, r;

  if (!v1) {
    r = (uhigh << halfbits) | u1;
    q1 = r / v;
    r  = r % v;
    r = (r << halfbits) | u0;
    q0 = r / v;
    r  = r % v;
  } else {
    const T v0 = v & lowmask;
    q1 = uhigh / v1;
    r  = uhigh % v1;
    while ((q1 & highmask) || q1*v0 > ((r << halfbits) | u1)) {
      --q1;
      r += v1;
    }
    r = ((r << halfbits) | u1) - q1*v0;
    q0 = r / v1;
    r = r % v1;
    while ((q0 & highmask) || q0*v0 > ((r << halfbits) | u0)) {
      --q0;
      r += v1;
    }
    r = ((r << halfbits) | u0) - q0*v0;
  }

  return std::pair<T,T>((q1 << halfbits) | q0, r);
}


template <typename T>
T* shift_left(const T* first, const T* last, T* dst, std::ptrdiff_t n)
{
  if (last != first) {
    const unsigned int limbbits = boost::integer_traits<T>::digits;
    unsigned int shift = n % limbbits;
    n /= limbbits;
    dst += (last - first) + n;
    T* d = dst;
    if (shift != 0) {
      const unsigned int revshift = limbbits - shift;
      T t, k = 0;
      ++d;
      while (last != first) {
        t = *--last;
        *--d = (t >> revshift) | k;
        k = t << shift;
      }
      *--d = k;
      if (*dst) ++dst;
    } else {
      while (last != first)
        *--d = *--last;
    }
    while (n--) *--d = 0;
  }
  return dst;
}

template <typename T>
T* shift_right(const T* first, const T* last, T* dst, std::ptrdiff_t n)
{
  if (last != first) {
    const unsigned int limbbits = boost::integer_traits<T>::digits;
    const unsigned int shift = n % limbbits;
    n /= limbbits;
    if (n >= last - first) return dst;
    first += n;
    if (shift != 0) {
      const unsigned int revshift = limbbits - shift;
      T t, k = *first++ >> shift;
      while (first != last) {
        t = *first++;
        *dst++ = (t << revshift) | k;
        k = t >> shift;
      }
      if (k) *dst++ = k;
    } else {
      while (first != last)
        *dst++ = *first++;
    }
  }
  return dst;
}


} // lowlevel
} // multiprecision
} // sputsoft
} // com

#endif	/* _LOWLEVEL_HPP */
