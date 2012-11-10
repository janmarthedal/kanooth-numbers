/*
 * File:   kanooth/numbers/floating_point.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-10-12 08:39Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_FLOATING_POINT_HPP
#define _KANOOTH_NUMBERS_FLOATING_POINT_HPP

#include <kanooth/numbers/positive_float.hpp>
#include <kanooth/numbers/detail/floating_point/float_impl.hpp>
#include <kanooth/numbers/detail/default_ops.hpp>

namespace kanooth {
namespace numbers {

typedef detail::numb<detail::floatnum<positive_float> > floating_point;

} // namespace kanooth
} // namespace numbers


#endif // _KANOOTH_NUMBERS_FLOATING_POINT_HPP
