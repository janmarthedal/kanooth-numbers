/*
 * File:   math/numbers/natural_number.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_MATH_NUMBERS_NATURAL_NUMBER_HPP_

#include <sputsoft/math/numbers/generic/natural_number.hpp>
#include <sputsoft/math/numbers/detail/digit_array.hpp>
#include <sputsoft/math/numbers/generic/lowlevel.hpp>
#include <sputsoft/math/numbers/gmp/lowlevel.hpp>

/*namespace sputsoft {
namespace math {
namespace numbers {*/

namespace generic {

typedef unary<identity, natnum< midlevel< ::detail::digit_array<unsigned long>, lowlevel<unsigned long> > > > natural_number;

} // namespace generic

namespace gmp {

typedef ::detail::expr::unary< ::detail::ops::unary::identity,
  ::generic::natnum< ::generic::midlevel< ::detail::digit_array<unsigned long>,
  lowlevel > > > natural_number;

} // namespace generic

typedef ::generic::natural_number natural_number;

/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_NATURAL_NUMBER_HPP_

