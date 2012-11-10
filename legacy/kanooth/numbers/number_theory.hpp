/*
 * File:   kanooth/numbers/number_theory.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-06-01 16:17Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_NUMBER_THEORY_HPP
#define _KANOOTH_NUMBERS_NUMBER_THEORY_HPP

#include <kanooth/numbers/number_traits.hpp>

namespace kanooth {
namespace numbers {

// TODO: Optimize this. Use d = 2, 3, 6k-1, 6k+1 for k=1, 2, ...
//       and change condition to d^2 <= n (or equivalent)
template <typename NUM, typename Out>
void factorize(NUM n, Out out)
{
  NUM q, r;

  while (is_greater_or_equal(n, 2u)) {
    divrem(q, r, n, 2u);
    if (!r) {
      *out++ = 2u;
      n = q;
    } else
      break;
  }

  NUM d=3u;
  while (is_greater_or_equal(n, d)) {
    divrem(q, r, n, d);
    if (!r) {
      *out++ = d;
      n = q;
    } else
      add(d, d, 2u);
  }
}

template <typename T1, typename T2>
struct gcd_result
  : public common_type<typename detail::unary_result<detail::ops::unary::abs, T1>::type,
                       typename detail::unary_result<detail::ops::unary::abs, T2>::type> {};

template <typename T1, typename T2>
typename gcd_result<T1, T2>::type
gcd(const T1& v1, const T2& v2)
{
  typename gcd_result<T1, T2>::type a, b;
  abs(a, v1);
  abs(b, v2);
  while (true) {
    if (is_zero(b)) return a;
    rem(a, a, b);
    if (is_zero(a)) return b;
    rem(b, b, a);
  }
}

} // numbers
} // kanooth

#endif // _KANOOTH_NUMBERS_NUMBER_THEORY_HPP
