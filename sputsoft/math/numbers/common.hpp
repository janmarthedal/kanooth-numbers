/*
 * File:   math/numbers/common.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_COMMON_HPP_
#define _SPUTSOFT_MATH_NUMBERS_COMMON_HPP_

#include <boost/cstdint.hpp>

/*namespace sputsoft {
namespace math {
namespace numbers {*/

typedef boost::uint8_t U8;
typedef boost::uint16_t U16;
typedef boost::uint32_t U32;
#ifndef BOOST_NO_INT64_T
#define SPUTSOFT_HAS_U64
typedef boost::uint64_t U64;
#endif

/*template <typename U, typename V>
inline U quotient(const U& u, const V& v) { return u / v; }*/

/*template <typename U, typename V>
struct quotient_remainder {
  static inline V remainder(const U& u, const V& v) { return u % v; }
};

template <typename U, typename V>
inline V remainder(const U& u, const V& v) {
  return quotient_remainder<U, V>::remainder(u, v);
}*/

/*template <typename U, typename V>
inline std::pair<U, V> divrem(const U& u, const V& v)
{
  return std::make_pair(quotient(u, v), remainder(u, v));
}*/

/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_COMMON_HPP_

