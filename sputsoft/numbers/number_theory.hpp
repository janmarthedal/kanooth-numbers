/*
 * File:   sputsoft/numbers/number_theory.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-06-01 16:17Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_NUMBER_THEORY_HPP
#define _SPUTSOFT_NUMBERS_NUMBER_THEORY_HPP

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
void gcd(NUM& r, NUM a, NUM b)
{
  while (true) {
    if (is_zero(b)) { abs(r, a); break; }
    rem(a, a, b);
    if (is_zero(a)) { abs(r, b); break; }
    rem(b, b, a);
  }
}

template <typename NUM>
NUM gcd(NUM a, NUM b)
{
  NUM r;
  gcd(r, a, b);
  return r;
}

} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_NUMBER_THEORY_HPP
