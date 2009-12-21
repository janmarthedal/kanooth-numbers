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

#include <cstring>

namespace sputsoft {
namespace numbers {
namespace generic {

template <typename T>
class lowlevel {
private:

  // n >= 0
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
    return 0u;
  }

public:

  // n >= 0
  // rp == xp  or  {rp,n} and {xp,n} do not overlap
  static inline void copy(T* rp, const T* xp, const std::size_t n)
  {
    if (!n || rp == xp) return;
    std::memcpy(rp, xp, n*sizeof(T));
  }

  // n >= 0
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
  static T add(T* rp, const T* xp, const std::size_t xn,
               const T* yp, const std::size_t yn)
  {
    T carry = add_n(rp, xp, yp, yn);
    return add_1(rp + yn, xp + yn, xn - yn, carry);    
  }

  // n >= 0
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
  static T sub(T* rp, const T* xp, const std::size_t xn,
               const T* yp, const std::size_t yn)
  {
    T borrow = sub_n(rp, xp, yp, yn);
    return sub_1(rp + yn, xp + yn, xn - yn, borrow);
  }

};

} // namespace generic
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_GENERIC_LOWLEVEL_HPP_
