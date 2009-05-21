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
inline T mult_add_add(const T u, const T v, T& w, const T k)
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
    k = mult_add_add(*first++, v, *dst++, k);
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

} // lowlevel
} // multiprecision
} // sputsoft
} // com

#endif	/* _LOWLEVEL_HPP */
