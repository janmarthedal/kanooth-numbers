/*
 * File:   natural_number_algorithms.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-27 11:43Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _NATURAL_NUMBER_ALGORITHMS_HPP
#define _NATURAL_NUMBER_ALGORITHMS_HPP


namespace sputsoft {
namespace multiprecision {


template <typename NUM>
NUM string_to_natural_number(const std::string& digits, unsigned short base=10)
{
  NUM r = NUM::zero;
  if (base >= 2 && base <= 36) {
    NUM b(base);
    unsigned short v;
    for (std::string::const_iterator p=digits.begin(); p != digits.end(); ++p) {
      char c = *p;
      if (c >= '0' && c <= '9') v = c - '0';
      else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
      else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
      else break;
      r *= b;
      r += NUM(v);
    }
  }
  return r;
}


// TODO: Optimize this. Use d = 2, 3, 6k-1, 6k+1 for k=1, 2, ...
// and change condition to d^2 <= n (or equivalent)
template <typename NUM, typename Out>
void factorize(NUM n, Out out)
{
  NUM two=2;
  std::pair<NUM,NUM> divrem;

  while (two <= n) {
    divrem = NUM::divide(n, two);
    if (divrem.second.isZero()) {
      *out++ = two;
      n = divrem.first;
    } else
      break;
  }

  NUM d = 3;
  while (d <= n) {
    divrem = NUM::divide(n, d);
    if (divrem.second.isZero()) {
      *out++ = d;
      n = divrem.first;
    } else
      d += two;
  }
}

template <typename NUM>
NUM factorial(unsigned n)
{
  NUM r(1);
  for (unsigned k=2; k <= n; ++k)
    r *= k;
  return r;
}

template <typename NUM>
NUM power(NUM n, unsigned p)
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


} // multiprecision
} // sputsoft


#endif	/* _NATURAL_NUMBER_ALGORITHMS_HPP */
