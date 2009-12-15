/*
 * File:   math/numbers/gmp/lowlevel.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_GMP_LOWLEVEL_HPP_
#define _SPUTSOFT_MATH_NUMBERS_GMP_LOWLEVEL_HPP_

#include <cstring>
#include <gmp.h>

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace gmp {

class lowlevel {
public:

  // n >= 0
  // rp == xp  or  {rp,n} and {xp,n} do not overlap
  static inline void copy(mp_limb_t* rp, const mp_limb_t* xp,
                          const std::size_t n)
  {
    if (!n || rp == xp) return;
    std::memcpy(rp, xp, n*sizeof(mp_limb_t));
  }

  // n >= 0
  static inline mp_limb_t add_1(mp_limb_t* rp,
                                const mp_limb_t* xp, std::size_t n,
                                const mp_limb_t y)
  { return mpn_add_1(rp, xp, n, y); }

  // xn >= yn >= 0
  static inline mp_limb_t add(mp_limb_t* rp,
                              const mp_limb_t* xp, const std::size_t xn,
                              const mp_limb_t* yp, const std::size_t yn)
  { return mpn_add(rp, xp, xn, yp, yn); }

};

} // namespace gmp
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_GENERIC_LOWLEVEL_HPP_

