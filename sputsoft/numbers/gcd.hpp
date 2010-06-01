/*
 * File:   sputsoft/numbers/gcd.hpp
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

#ifndef _SPUTSOFT_NUMBERS_GCD_HPP
#define _SPUTSOFT_NUMBERS_GCD_HPP

namespace sputsoft {
namespace numbers {

template <typename NUM>
NUM gcd(NUM a, NUM b)
{
  while (true) {
    if (is_zero(b)) return abs(a);
    rem(a, a, b);
    if (is_zero(a)) return abs(b);
    rem(b, b, a);
  }
}

} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_GCD_HPP
