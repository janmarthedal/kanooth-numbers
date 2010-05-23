/* 
 * File:   sputsoft_numbers_integer.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-06 8:11Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_INTEGER_HPP
#define _SPUTSOFT_NUMBERS_INTEGER_HPP

#include <sputsoft/numbers/natural_number.hpp>
#include <sputsoft/numbers/detail/int_impl.hpp>

namespace sputsoft {
namespace numbers {

typedef detail::numb<detail::intnum<natural_number> > integer;

} // namespace sputsoft
} // namespace numbers


#endif // _SPUTSOFT_NUMBERS_INTEGER_HPP
