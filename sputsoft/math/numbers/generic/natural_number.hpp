/*
 * File:   math/numbers/generic/natural_number.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_MATH_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_

#include <sputsoft/math/numbers/detail/natural_number.hpp>
#include <sputsoft/math/numbers/detail/digit_array.hpp>
#include <sputsoft/math/numbers/detail/midlevel.hpp>
#include <sputsoft/math/numbers/generic/lowlevel.hpp>

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace generic {

typedef ::detail::expr<
            ::detail::natural_number<
                ::detail::midlevel<
                    ::detail::digit_array<unsigned long>,
                    lowlevel<unsigned long> > >,
            ::detail::natural_number<
                ::detail::midlevel<
                    ::detail::digit_array<unsigned long>,
                    lowlevel<unsigned long> > >
        > natural_number;

} // namespace generic
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_

