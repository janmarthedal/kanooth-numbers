/*
 * File:   numbers/generic/natural_number.hpp
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

#ifndef _SPUTSOFT_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_

#include <sputsoft/numbers/detail/natural_number.hpp>
#include <sputsoft/numbers/detail/digit_array.hpp>
#include <sputsoft/numbers/detail/midlevel.hpp>
#include <sputsoft/numbers/generic/lowlevel.hpp>

namespace sputsoft {
namespace numbers {
namespace generic {

typedef unsigned long digit_type;

typedef detail::expr<
            detail::natural_number<
                detail::midlevel<
                    detail::digit_array<digit_type>,
                    lowlevel<digit_type> > > > natural_number;

} // namespace generic
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_
