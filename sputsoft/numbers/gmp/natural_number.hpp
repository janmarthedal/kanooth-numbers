/*
 * File:   numbers/gmp/natural_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_GMP_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_NUMBERS_GMP_NATURAL_NUMBER_HPP_

#include <sputsoft/numbers/detail/natural_number.hpp>
#include <sputsoft/numbers/detail/digit_array.hpp>
#include <sputsoft/numbers/detail/midlevel.hpp>
#include <sputsoft/numbers/gmp/lowlevel.hpp>

namespace sputsoft {
namespace numbers {
namespace gmp {

typedef detail::expr<
            detail::natural_number_base<
                detail::midlevel<
                    detail::digit_array<mp_limb_t>,
                    lowlevel > > > natural_number;

std::ostream& operator<<(std::ostream& os, const natural_number& n)
{
  if (n) {
    typedef natural_number::container_type container_type;
    container_type tmp(n.get_container());
    unsigned max_digits = tmp.size() * GMP_LIMB_BITS / 3 + 1;
    char st[max_digits];
    std::size_t used = mpn_get_str((unsigned char*)st, 10, tmp.get(), tmp.size());
    char *p = st;
    while (!*p) { ++p; --used; }
    for (unsigned i=0; i < used; ++i) p[i] += '0';
    p[used] = 0;
    os << p;
  } else
    os << "0";
  return os;
}


} // namespace gmp
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP_
