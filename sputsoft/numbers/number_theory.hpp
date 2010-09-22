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

#include <sputsoft/numbers/common_type.hpp>

namespace sputsoft {
namespace numbers {

// TODO: Optimize this. Use d = 2, 3, 6k-1, 6k+1 for k=1, 2, ...
//       and change condition to d^2 <= n (or equivalent)
/*template <typename NUM, typename Out>
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
}*/

template <typename T1, typename T2>
struct gcd_return_type
  : public common_type<
             typename detail::unary_result<detail::ops::unary::abs, T1>::return_type,
             typename detail::unary_result<detail::ops::unary::abs, T2>::return_type> {};

template <typename R, typename T1, typename T2>
void gcd(R& r, const T1& v1, const T2& v2)
{
  typename gcd_return_type<T1, T2>::type a, b;
  abs(a, v1);
  abs(b, v2);
  while (true) {
    if (is_zero(b)) { set(r, a); break; }
    rem(a, a, b);
    if (is_zero(a)) { set(r, b); break; }
    rem(b, b, a);
  }
}

template <typename T1, typename T2>
typename gcd_return_type<T1, T2>::type gcd(const T1& a, const T2& b)
{
  typename gcd_return_type<T1, T2>::type r;
  gcd(r, a, b);
  return r;
}

} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_NUMBER_THEORY_HPP
