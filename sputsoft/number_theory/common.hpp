/*
 * File:   number_theory/common.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-16 13:46Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBER_THEORY_COMMON_HPP_
#define _SPUTSOFT_NUMBER_THEORY_COMMON_HPP_

#include <cstdlib>

namespace sputsoft {
namespace number_theory {

template <typename T>
struct floor_log2_evaluator {
  static std::size_t floor_log2(T n) {
    std::size_t r = -1;
    while (n) { ++r; n >>= 1; }
    return r;
  }
};

template <typename T>
inline std::size_t floor_log2(T n) {
  return floor_log2_evaluator<T>::floor_log2(n);
}


template <typename U, typename V>
struct quotrem_evaluator {
  static std::pair<U, V> quotrem(const U& u, const V& v)
  { return std::make_pair(u / v, u % v); }
};

template <typename U, typename V>
inline std::pair<U, V> quotrem(const U& u, const V& v)
{
  return quotrem_evaluator<U, V>::quotrem(u, v);
}

template <>
struct quotrem_evaluator<long, long> {
  static std::pair<long, long> quotrem(long u, long v) {
    std::ldiv_t qr = ldiv(u, v);
    return std::make_pair(qr.quot, qr.rem);
  }
};

template <>
struct quotrem_evaluator<int, int> {
  static std::pair<int, int> quotrem(int u, int v) {
    std::div_t qr = div(u, v);
    return std::make_pair(qr.quot, qr.rem);
  }
};

} // namespace number_theory
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBER_THEORY_COMMON_HPP_
