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

template <typename T1, typename T2>
struct mul_power_result : detail::binary_result<detail::ops::binary::mul, T1, T2> {};

// r = y * z^p
template <typename T1, typename T2>
typename mul_power_result<T1, T2>::type
mul_power(const T1& y_, const T2& z_, std::size_t p)
{
  typename mul_power_result<T1, T2>::type y, z;
  set(y, y_);
  set(z, z_);
  while (p) {
    if (p & 1) {
      mul(y, y, z);
      p--;
    } else {
      mul(z, z, z);
      p /= 2;
    }
  }
  return y;
}

template <typename T>
T power(const T& x, std::size_t p)
{
  return mul_power(1u, x, p);
}

namespace {

/*template <typename T>
T floor_sqrt_help(const T& n)
{
  if (n <= 1u) return n;
  std::size_t k = floor_log2(n);
  T a = T(1u) << (k/2+1), b;
  int it = 0;
  while (true) {
    b = (a + n/a) >> 1;
    it++;
    //if (b >= a) return a;
    if (b >= a) { std::cout << "Iterations: " << it << std::endl; return a; }
    a = (b + n/b) >> 1;
    it++;
    //if (a >= b) return b;
    if (a >= b) { std::cout << "Iterations: " << it << std::endl; return b; }
  }
}*/

template <typename T>
T floor_sqrt_help(const T& n)
{
  if (n <= 1u) return n;
  std::size_t k = floor_log2(n);
  T a = T(1u) << (k/2), b = a << 1, m;
  int it = 0;
  while (true) {
    it++;
    m = (a + b) >> 1;
    if (a == m)
      { std::cout << "Iterations: " << it << std::endl; return a; }
    if (m*m > n)
      b = m;
    else
      a = m;
  }
}

}

template <typename T>
struct floor_sqrt_result : type_if<is_integral<T>::value, typename eval_result<T>::type> {};

template <typename T>
typename floor_sqrt_result<T>::type floor_sqrt(const T& n)
{
  return floor_sqrt_help((typename floor_sqrt_result<T>::type) n);
}

template <typename NUM>
NUM sideways_sum(NUM n, unsigned base)
{
  NUM res;
  while (n)
    add(res, res, divrem(n, n, base));
  return res;
}

} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_COMMON_FUNCTIONS_HPP
