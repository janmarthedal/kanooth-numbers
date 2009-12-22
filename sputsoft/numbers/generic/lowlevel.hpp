/*
 * File:   numbers/generic/lowlevel.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_GENERIC_LOWLEVEL_HPP_
#define _SPUTSOFT_NUMBERS_GENERIC_LOWLEVEL_HPP_

namespace sputsoft {
namespace numbers {
namespace generic {

class lowlevel {
private:

  // n >= 0
  template <typename T>
  static inline T add_n(T* rp, const T* xp, const T* yp, std::size_t n)
  {
    T lx, ly, lz;
    bool carry = false;
    while (n--) {
      lx = *xp++;
      ly = *yp++;
      if (carry) {
        lz = lx + ly + 1;
        carry = lz <= lx;
      } else {
        lz = lx + ly;
        carry = lz < lx;
      }
      *rp++ = lz;
    }
    return carry ? 1 : 0;
  }

  // n >= 0
  template <typename T>
  static inline T sub_n(T* rp, const T* xp, const T* yp, std::size_t n)
  {
    T lx, ly, lz;
    bool borrow = false;
    while (n--) {
      lx = *xp++;
      ly = *yp++;
      if (borrow) {
        lz = lx - ly - 1;
        borrow = lx <= ly;
      } else {
        lz = lx - ly;
        borrow = lx < ly;
      }
      *rp++ = lz;
    }
    return borrow ? 1 : 0;
  }

  // n >= 0
  template <typename T>
  static inline T inc(T* rp, const T* xp, std::size_t n)
  {
    while (n && *xp == T(-1)) {
      *rp = 0;
      ++rp; ++xp; --n;
    }
    if (!n) return 1;
    *rp = *xp + 1;
    ++rp; ++xp; --n;
    copy(rp, xp, n);
    return 0;
  }

  // n >= 0
  template <typename T>
  static inline T dec(T* rp, const T* xp, std::size_t n)
  {
    while (n && !*xp) {
      *rp = T(-1);
      ++rp; ++xp; --n;
    }
    if (!n) return 1;
    *rp = *xp - 1;
    ++rp; ++xp; --n;
    copy(rp, xp, n);
    return 0;
  }

  template <typename T>
  static inline void double_mult_add_add(const T u, const T v, const T a1, const T a2, T& low, T& high)
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
  static inline void double_mult_add(const T u, const T v, const T a, T& low, T& high)
  {
    double_mult_add_add(u, v, a, T(0), low, high);
  }

  template <typename T>
  static inline T multiply_sequence_with_digit(const T* first, const T* last, T* dst, T v)
  {
    T k = 0;
    while (first != last) {
      double_mult_add(*first++, v, k, *dst, k);
      dst++;
    }
    return k;
  }

  template <typename T>
  static inline T multiply_add_sequence_with_digit(const T* first, const T* last, T* dst, T v)
  {
    T k = 0;
    while (first != last) {
      double_mult_add_add(*first++, v, *dst, k, *dst, k);
      dst++;
    }
    return k;
  }

  // Assumes ufirst != ulast and vfirst != vlast
  template <typename T>
  static inline void multiply_sequences(const T* ufirst, const T* ulast,
      const T* vfirst, const T* vlast, T* wfirst)
  {
    T *wlast = wfirst + (ulast - ufirst);
    *wlast++ = multiply_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
    while (vfirst != vlast)
      *wlast++ = multiply_add_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
  }

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
      }
      r = ((r << halfbits) | u0) - q0*v0;
    }

    quot = (q1 << halfbits) | q0;
    rem = r;
  }

public:

  // n >= 0
  // rp == xp  or  {rp,n} and {xp,n} do not overlap
  template <typename T>
  static inline void copy(T* z1, const T* x1, const std::size_t n)
  {
    if (!n || z1 == x1) return;
    std::copy(x1, x1 + n, z1);
  }

  // n >= 0
  template <typename T>
  static inline T add_1(T* rp, const T* xp, std::size_t n, const T y)
  {
    if (!n) return y;
    if (y) {
      T z = *xp + y;
      *rp = z;
      ++rp; ++xp; --n;
      if (z < y)
        return inc(rp, xp, n);
    }
    copy(rp, xp, n);
    return 0;
  }

  // xn >= yn >= 0
  template <typename T>
  static T add(T* rp, const T* xp, const std::size_t xn,
               const T* yp, const std::size_t yn)
  {
    T carry = add_n(rp, xp, yp, yn);
    return add_1(rp + yn, xp + yn, xn - yn, carry);    
  }

  // n >= 0
  template <typename T>
  static inline T sub_1(T* rp, const T* xp, std::size_t n, const T y)
  {
    if (!n) return y;
    if (y) {
      T x = *xp;
      *rp = x - y;
      ++rp; ++xp; --n;
      if (x < y)
        return dec(rp, xp, n);
    }
    copy(rp, xp, n);
    return 0;
  }

  // xn >= yn >= 0
  template <typename T>
  static T sub(T* rp, const T* xp, const std::size_t xn,
               const T* yp, const std::size_t yn)
  {
    T borrow = sub_n(rp, xp, yp, yn);
    return sub_1(rp + yn, xp + yn, xn - yn, borrow);
  }

  // n >= 0
  template <typename T>
  static inline T mul_1(T* rp, const T* xp, std::size_t n, const T y) {
    return multiply_sequence_with_digit(xp, xp+n, rp, y);
  }

  // xn >= yn >= 1
  // {rp, xn+yn}, {xp, xn} don't overlap, {rp, xn+yn}, {yp, yn} don't overlap
  template <typename T>
  static inline void mul(T* rp, const T* xp, const std::size_t xn,
                         const T* yp, const std::size_t yn) {
    multiply_sequences(xp, xp+xn, yp, yp+yn, rp);
  }

};

} // namespace generic
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_GENERIC_LOWLEVEL_HPP_
