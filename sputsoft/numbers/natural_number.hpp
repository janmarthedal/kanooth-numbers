/*
 * File:   numbers/natural_number.hpp
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

#ifndef _SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP_

#include <sputsoft/numbers/generic/natural_number.hpp>
#include <sputsoft/numbers/generic/natural_number_shared.hpp>
#ifdef SPUTSOFT_USE_GMP
#include <sputsoft/numbers/gmp/natural_number.hpp>
#endif

namespace sputsoft {
namespace numbers {

#ifdef SPUTSOFT_USE_GMP
typedef gmp::natural_number natural_number;
#else
typedef generic::natural_number natural_number;
#endif

} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP_
