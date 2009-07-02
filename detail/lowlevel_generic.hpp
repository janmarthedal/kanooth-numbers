/*
 * File:   lowlevel_generic.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-23 08:25Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */


#ifndef _LOWLEVEL_GENERIC_HPP
#define _LOWLEVEL_GENERIC_HPP


#include <boost/integer_traits.hpp>


/*namespace sputsoft {
namespace multiprecision {*/
namespace lowlevel {


template <typename T>
inline unsigned lg_floor(T n)
{
  unsigned r = -1;
  while (n) {
    n >>= 1;
    ++r;
  }
  return r;
}


/*
 * LEVEL 1 - CONSTANT TIME OPERATIONS
 */

template <typename T>
inline void double_mult_add_add(const T u, const T v, const T a1, const T a2, T& low, T& high)
{
  const unsigned int halfbits = boost::integer_traits<T>::digits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  T u1 = u >> halfbits, u0 = u & lowmask;
  T v1 = v >> halfbits, v0 = v & lowmask;
  T r  = u0*v0 + (a1 & lowmask) + (a2 & lowmask);
  T s  = u1*v0 + (a1 >> halfbits) + (r >> halfbits);
  T t  = u0*v1 + (s & lowmask) + (a2 >> halfbits);
  high = u1*v1 + (s >> halfbits) + (t >> halfbits);
  low  = (t << halfbits) | (r & lowmask);
}

template <typename T>
inline void double_mult_add(const T u, const T v, const T a, T& low, T& high)
{
  double_mult_add_add(u, v, a, (T) 0, low, high);
}

/*
 * Used by: NonNegativeInteger::long_divide
 */
template <typename T>
inline void double_mult(const T u, const T v, T& low, T& high)
{
  double_mult_add_add(u, v, (T) 0, (T) 0, low, high);
}

template <typename T>
inline T mult_sub_sub(T& w, const T u, const T v, T k)
{
  T wt;
  double_mult_add(u, v, k, wt, k);
  if (wt > w) ++k;
  w -= wt;
  return k;
}

/*
 * Used by: NonNegativeInteger::simple_divide, NonNegativeInteger::long_divide
 */
template <typename T>
void double_div(T uhigh, T ulow, T v, T& quot, T& rem)
{
  const unsigned int digitbits = boost::integer_traits<T>::digits;
  const unsigned int halfbits = digitbits / 2;
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
      if (r & highmask) break;
    }
    r = ((r << halfbits) | u1) - q1*v0;
    q0 = r / v1;
    r = r % v1;
    while ((q0 & highmask) || q0*v0 > ((r << halfbits) | u0)) {
      --q0;
      r += v1;
      if (r & highmask) break;
      //if (r & highmask) { quot = 0; rem = (T) -1; return; }
    }
    r = ((r << halfbits) | u0) - q0*v0;
  }

  quot = (q1 << halfbits) | q0;
  rem = r;
}


/*
 * LEVEL 2 - LINEAR TIME, ONE SEQUENCE
 */

/*
 * Used by: multiply_sequences
 */
template <typename T>
inline T multiply_add_sequence_with_digit(const T* first, const T* last, T* dst, T v)
{
  T k = 0;
  while (first != last) {
    double_mult_add_add(*first++, v, *dst, k, *dst, k);
    dst++;
  }
  return k;
}

/*
 * Used by: NonNegativeInteger::operator*=, multiply_sequences
 */
template <typename T>
inline T multiply_sequence_with_digit(const T* first, const T* last, T* dst, T v)
{
  T k = 0;
  while (first != last) {
    double_mult_add(*first++, v, k, *dst, k);
    dst++;
  }
  return k;
}

/*
 * Used by: NonNegativeInteger::long_divide
 */
template <typename T>
T sequence_mult_digit_sub(T* ufirst, T* ulast, const T* vfirst, T q)
{
  T k = 0;
  while (ufirst != ulast)
    k = mult_sub_sub(*ufirst++, *vfirst++, q, k);
  return k;
}

/*
 * Used by: NonNegativeInteger::long_divide, NonNegativeInteger::shift_left, NonNegativeInteger::shift_left_this
 */
template <typename T>
T* shift_left(const T* first, const T* last, T* dst, std::ptrdiff_t n)
{
  if (last != first) {
    const unsigned int digitbits = boost::integer_traits<T>::digits;
    unsigned int shift = n % digitbits;
    n /= digitbits;
    dst += (last - first) + n;
    T* d = dst;
    if (shift != 0) {
      const unsigned int revshift = digitbits - shift;
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

/*
 * Used by: NonNegativeInteger::long_divide, NonNegativeInteger::shift_right, NonNegativeInteger::shift_right_this
 */
template <typename T>
T* shift_right(const T* first, const T* last, T* dst, std::ptrdiff_t n)
{
  if (last != first) {
    const unsigned int digitbits = boost::integer_traits<T>::digits;
    const unsigned int shift = n % digitbits;
    n /= digitbits;
    if (n >= last - first) return dst;
    first += n;
    if (shift != 0) {
      const unsigned int revshift = digitbits - shift;
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


/*
 * LEVEL 2 - LINEAR TIME, TWO SEQUENCES
 */

/*
 * Assumes ulast-ufirst > vlast-vfirst
 *
 * Used by: add_sequences, NonNegativeInteger::long_divide
 */
template <typename T>
T* add_sequences_with_overflow(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst, bool& overflow)
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
  overflow = carry;
  return dst;
}

/*
 * Used by: NonNegativeInteger::add, NonNegativeInteger::operator+=
 */
template <typename T>
T* add_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst)
{
  bool overflow;
  dst = (ulast - ufirst > vlast - vfirst) ?
          add_sequences_with_overflow(ufirst, ulast, vfirst, vlast, dst, overflow)
        : add_sequences_with_overflow(vfirst, vlast, ufirst, ulast, dst, overflow);
  if (overflow) *dst++ = 1;
  return dst;
}


/*
 * Requires last1-first1 >= last2-first2
 *
 * Used by: NonNegativeInteger::subtract, NonNegativeInteger::operator-=
 *
 */
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
 * Used by: ?
 */
template <typename T>
T* and_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst)
{
  T* dfirst = dst;
  if (ulast - ufirst > vlast - vfirst) {
    while (vfirst != vlast)
      *dst++ = *ufirst++ & *vfirst++;
  } else {
    while (ufirst != ulast)
      *dst++ = *ufirst++ & *vfirst++;
  }
  while (dst != dfirst && !*(dst-1))
    --dst;
  return dst;
}


/*
 * Used by: ?
 */
template <typename T>
T* or_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst)
{
  if (ulast - ufirst > vlast - vfirst) {
    while (vfirst != vlast)
      *dst++ = *ufirst++ | *vfirst++;
    dst = std::copy(ufirst, ulast, dst);
  } else {
    while (ufirst != ulast)
      *dst++ = *ufirst++ | *vfirst++;
    dst = std::copy(vfirst, vlast, dst);
  }
  return dst;
}


/*
 * Used by: ?
 */
template <typename T>
T* xor_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* dst)
{
  if (ulast - ufirst > vlast - vfirst) {
    while (vfirst != vlast)
      *dst++ = *ufirst++ ^ *vfirst++;
    dst = std::copy(ufirst, ulast, dst);
  } else if (ulast - ufirst < vlast - vfirst) {
    while (ufirst != ulast)
      *dst++ = *ufirst++ ^ *vfirst++;
    dst = std::copy(vfirst, vlast, dst);
  } else {
    T* dfirst = dst;
    while (ufirst != ulast)
      *dst++ = *ufirst++ ^ *vfirst++;
    while (dst != dfirst && !*(dst-1))
      --dst;
  }
  return dst;
}


/*
 * LEVEL 3 - QUADRATIC TIME
 */

/*
 * Assumes ufirst != ulast and vfirst != vlast
 *
 * Used by: NonNegativeInteger::multiply
 */
template <typename T>
T* multiply_sequences(const T* ufirst, const T* ulast, const T* vfirst, const T* vlast, T* wfirst)
{
  T *wlast = wfirst + (ulast - ufirst);
  *wlast++ = multiply_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
  while (vfirst != vlast)
    *wlast++ = multiply_add_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
  if (!*(wlast-1)) --wlast;
  return wlast;
}


} // lowlevel
/*} // multiprecision
} // sputsoft*/

#endif	/* _LOWLEVEL_GENERIC_HPP */
