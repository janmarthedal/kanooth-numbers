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

// TODO: Optimize this. Use d = 2, 3, 6k-1, 6k+1 for k=1, 2, ...
//       and change condition to d^2 <= n (or equivalent)
template <typename NUM, typename Out>
void factorize(NUM n, Out out)
{
  NUM two=2, q, r;

  while (two <= n) {
    quotrem(q, r, n, two);
    if (!r) {
      *out++ = two;
      n = q;
    } else
      break;
  }

  NUM d=3;
  while (d <= n) {
    quotrem(q, r, n, d);
    if (!r) {
      *out++ = d;
      n = q;
    } else
      d += two;
  }
}

template <typename NUM>
NUM factorial(std::size_t n)
{
  NUM r(1);
  for (std::size_t k=2; k <= n; ++k)
    r *= k;
  return r;
}

template <typename NUM>
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

} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_COMMON_FUNCTIONS_HPP
