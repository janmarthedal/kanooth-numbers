/*
 * File:   sputsoft/numbers/detail/lowlevel.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_LOWLEVEL_GMP_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_LOWLEVEL_GMP_HPP_

#include <gmp.h>

namespace sputsoft {
namespace numbers {
namespace detail {

struct lowlevel_gmp {

  typedef mp_limb_t digit_type;

  // n >= 0
  static inline void copy_forward(digit_type* rp, const digit_type* xp,
                                  const std::size_t n) {
    if (n && rp != xp)
#if __GNU_MP__ >= 5
      mpn_copyi(rp, xp, n);
#else
      std::copy(xp, xp + n, rp);
#endif
  }

  // n >= 0
  static inline void copy_backward(digit_type* rp, const digit_type* xp,
                                   const std::size_t n) {
    if (n && rp != xp)
#if __GNU_MP__ >= 5
      mpn_copyd(rp, xp, n);
#else
      std::copy_backward(xp, xp + n, rp + n);
#endif
  }

  static inline void fill_zero(digit_type* rp, const std::size_t n) {
#if __GNU_MP__ >= 5
    mpn_zero(rp, n);
#else
    std::fill_n(rp, n, 0);
#endif
  }

  // n >= 0
  static inline digit_type add_1(digit_type* rp,
                                 const digit_type* xp, std::size_t n,
                                 const digit_type y) {
    return mpn_add_1(rp, xp, n, y);
  }

  // xn >= yn >= 0
  static inline digit_type add(digit_type* rp,
                               const digit_type* xp, const std::size_t xn,
                               const digit_type* yp, const std::size_t yn) {
    return mpn_add(rp, xp, xn, yp, yn);
  }

  // n >= 1
  static inline void sub_1(digit_type* rp,
                           const digit_type* xp, std::size_t n,
                           const digit_type y) {
    mpn_sub_1(rp, xp, n, y);
  }

  // xn >= yn >= 0
  static inline void sub(digit_type* rp,
                         const digit_type* xp, const std::size_t xn,
                         const digit_type* yp, const std::size_t yn) {
    mpn_sub(rp, xp, xn, yp, yn);
  }

  // n >= 0
  static inline digit_type mul_1(digit_type* rp,
                                 const digit_type* xp, std::size_t n,
                                 const digit_type y) {
    return mpn_mul_1(rp, xp, n, y);
  }

  // xn >= yn >= 0
  // {rp, xn+yn}, {xp, xn} don't overlap, {rp, xn+yn}, {yp, yn} don't overlap
  static inline void mul(digit_type* rp,
                         const digit_type* xp, const std::size_t xn,
                         const digit_type* yp, const std::size_t yn) {
    mpn_mul(rp, xp, xn, yp, yn);
  }

  // n >= 0
  // rp == xp  or  {rp,n} and {xp,n} do not overlap
  static inline digit_type quotrem_1(digit_type* rp,
                                     const digit_type* xp, std::size_t n,
                                     const digit_type y) {
    return mpn_divrem_1(rp, 0, xp, n, y);
  }

  // un >= vn >= 1, vp[vn-1] != 0
  // {qp, un-vn+1}, {rp, vn}, {up, un}, {vp, vn} do not overlap
  static inline void quotrem(digit_type* qp, digit_type* rp,
                             const digit_type* up, std::size_t un,
                             const digit_type* vp, std::size_t vn) {
    mpn_tdiv_qr(qp, rp, 0, up, un, vp, vn);
  }

  // xn >= 0
  static inline digit_type rem_1(const digit_type* xp, const std::size_t xn,
                                 digit_type y) {
    return mpn_mod_1(xp, xn, y);
  }

  // zp >= xp, 1 <= count < digit_bits
  static inline digit_type lshift(digit_type* zp,
                                  const digit_type* xp, std::size_t n,
                                  unsigned count) {
    return mpn_lshift(zp, xp, n, count);
  }

  // zp <= xp, 1 <= count < digit_bits
  static inline digit_type rshift(digit_type* zp,
                                  const digit_type* xp, std::size_t n,
                                  unsigned count) {
    return mpn_rshift(zp, xp, n, count);
  }

  static inline int comp(const digit_type* xp, const digit_type* yp, std::size_t n) {
    return mpn_cmp(xp, yp, n);
  }

  static void bitwise_and(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
#if __GNU_MP__ >= 5
    mpn_and_n(zp, xp, yp, n);
#else
    while (n--)
      *zp++ = *xp++ & *yp++;
#endif
  }

  static void bitwise_or(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
#if __GNU_MP__ >= 5
    mpn_ior_n(zp, xp, yp, n);
#else
    while (n--)
      *zp++ = *xp++ | *yp++;
#endif
  }

  static void bitwise_xor(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
#if __GNU_MP__ >= 5
    mpn_xor_n(zp, xp, yp, n);
#else
    while (n--)
      *zp++ = *xp++ ^ *yp++;
#endif
  }

  static void bitwise_and_not(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
#if __GNU_MP__ >= 5
    mpn_andn_n(zp, xp, yp, n);
#else
    while (n--)
      *zp++ = *xp++ & ~*yp++;
#endif
  }

};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_LOWLEVEL_GMP_HPP_
