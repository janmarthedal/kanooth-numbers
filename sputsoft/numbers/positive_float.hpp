/*
 * File:   sputsoft/numbers/positive_float.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-08-24 08:39Z
 *
 * (C) Copyright SputSoft 2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_POSITIVE_FLOAT_HPP
#define _SPUTSOFT_NUMBERS_POSITIVE_FLOAT_HPP

#include <sputsoft/numbers/natural_number.hpp>
#include <sputsoft/numbers/detail/positive_float/impl.hpp>
#include <sputsoft/numbers/detail/default_ops.hpp>

namespace sputsoft {
namespace numbers {

typedef detail::numb<detail::posfloatnum<natural_number, std::ptrdiff_t, 64> > positive_float;

} // namespace sputsoft
} // namespace numbers


#endif // _SPUTSOFT_NUMBERS_POSITIVE_FLOAT_HPP
