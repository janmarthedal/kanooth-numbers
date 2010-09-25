/*
 * File:   sputsoft/numbers/common_functions.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-27 11:43Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_COMMON_FUNCTIONS_HPP
#define _SPUTSOFT_NUMBERS_COMMON_FUNCTIONS_HPP

namespace sputsoft {
namespace numbers {

template <typename NUM>
NUM factorial(std::size_t n)
{
  NUM r = 1u;
  for (std::size_t k=2; k <= n; ++k)
    mul(r, r, k);
  return r;
}

/*template <typename NUM>
NUM power(NUM n, std::size_t p)
{
  NUM y = 1, z = n;
  while (p) {
    if (p & 1) {
      y *= z;
      p--;
    } else {
      z *= z;
      p /= 2;
    }
  }
  return y;
}

template <typename NUM>
NUM sqrt_floor(NUM n)
{
  if (n <= 1) return n;
  NUM a = n, b;
  while (true) {
    b = (a + n/a)/2;
    if (b >= a) return a;
    a = (b + n/b)/2;
    if (a >= b) return b;
  }
}

template <typename R, typename E>
inline R sqrt_floor(const detail::expr<R, E>& ex)
{
  return sqrt_floor((R) ex);
}

template <typename NUM>
NUM sideways_sum(NUM n, unsigned base)
{
  NUM res;
  while (n)
    res += quotrem(n, n, base);
  return res;
}
*/

} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_COMMON_FUNCTIONS_HPP
