/*
 * File:   lowlevel.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created on 19. maj 2009, 16:39
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _LOWLEVEL_HPP
#define _LOWLEVEL_HPP

#include <boost/tuple/tuple.hpp>

namespace com {
namespace sputsoft {
namespace multiprecision {
namespace lowlevel {

/*
 * ADDITION
 */

template <typename T>
inline boost::tuple<bool, const T*, T*> seq_plus_seq_to_seq(const T* first1, const T* last1, const T* first2, T* dst)
{
  T lx, ly, lz;
  bool carry = false;
  for (; first1 != last1; ++first1, ++first2, ++dst) {
    lx = *first1;
    ly = *first2;
    if (carry) {
      lz = lx + ly + 1;
      carry = lz <= lx;
    } else {
      lz = lx + ly;
      carry = lz < lx;
    }
    *dst = lz;
  }
  return boost::make_tuple(carry, first2, dst);
}

template <typename T>
inline boost::tuple<bool, T*> seq_plus_one_to_seq(const T* first, const T* last, T* dst)
{
  T lz;
  bool carry = true;
  for (; first != last && carry; ++first, ++dst) {
    lz = *first + 1;
    *dst = lz;
    carry = !lz;
  }
  if (!carry) std::copy(first, last, dst);
  return boost::make_tuple(carry, dst);
}

template <typename T>
T* adder(const T* first1, const T* last1, const T* first2, const T* last2, T* dst)
{
  if (last1 - first1 > last2 - first2) {
    swap(first1, first2);
    swap(last1, last2);
  }
  bool carry;
  boost::tie(carry, first2, dst) = seq_plus_seq_to_seq(first1, last1, first2, dst);
  if (carry) {
    boost::tie(carry, dst) = seq_plus_one_to_seq(first2, last2, dst);
    if (carry) *dst++ = 1;
  } else
    dst = std::copy(first2, last2, dst);
  return dst;
}


} // lowlevel
} // multiprecision
} // sputsoft
} // com

#endif	/* _LOWLEVEL_HPP */
