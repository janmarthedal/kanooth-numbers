/*
 * File:   kanooth/numbers/rational_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-10-05 11:28Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_RATIONAL_NUMBER_HPP
#define _KANOOTH_NUMBERS_RATIONAL_NUMBER_HPP

#include <kanooth/numbers/natural_number.hpp>
#include <kanooth/numbers/integer.hpp>
#include <kanooth/numbers/detail/rational_number/rat_num_impl.hpp>
#include <kanooth/numbers/detail/default_ops.hpp>

namespace kanooth {
namespace numbers {

typedef detail::numb<detail::ratnum<integer, natural_number> > rational_number;

} // namespace kanooth
} // namespace numbers


#endif // _KANOOTH_NUMBERS_RATIONAL_NUMBER_HPP
