/*
 * File:   kanooth/numbers/positive_float.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-08-24 08:39Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_POSITIVE_FLOAT_HPP
#define _KANOOTH_NUMBERS_POSITIVE_FLOAT_HPP

#include <kanooth/numbers/natural_number.hpp>
#include <kanooth/numbers/detail/positive_float/pos_float_impl.hpp>
#include <kanooth/numbers/detail/default_ops.hpp>

namespace kanooth {
namespace numbers {

typedef detail::numb<detail::posfloatnum<natural_number, std::ptrdiff_t, 64> > positive_float;

} // namespace kanooth
} // namespace numbers


#endif // _KANOOTH_NUMBERS_POSITIVE_FLOAT_HPP
