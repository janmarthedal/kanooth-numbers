/* 
 * File:   kanooth_numbers_integer.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-06 8:11Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_INTEGER_HPP
#define _KANOOTH_NUMBERS_INTEGER_HPP

#include <kanooth/numbers/natural_number.hpp>
#include <kanooth/numbers/detail/integer/int_impl.hpp>
#include <kanooth/numbers/detail/default_ops.hpp>
//#include <kanooth/numbers/detail/overload.hpp>

namespace kanooth {
namespace numbers {

typedef detail::numb<detail::intnum<natural_number> > integer;

} // namespace kanooth
} // namespace numbers


#endif // _KANOOTH_NUMBERS_INTEGER_HPP
