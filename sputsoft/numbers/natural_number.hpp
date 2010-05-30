/* 
 * File:   sputsoft/numbers/natural_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-03 20:12Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP
#define	_SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP

#include <sputsoft/numbers/detail/digit_array.hpp>
#include <sputsoft/numbers/detail/array_allocator.hpp>
#include <sputsoft/numbers/detail/nat_num_shared.hpp>
#include <sputsoft/numbers/detail/overload.hpp>

#ifdef SPUTSOFT_USE_GMP
#include <sputsoft/numbers/detail/lowlevel_gmp.hpp>
#else
#include <sputsoft/numbers/detail/lowlevel_generic.hpp>
#endif

namespace sputsoft {
namespace numbers {
namespace detail {

#ifdef SPUTSOFT_USE_GMP
typedef lowlevel_gmp lowlevel;
#else
typedef lowlevel_generic<array_allocator<void> > lowlevel;
#endif

} // namespace detail

typedef detail::numb<
          detail::natnum<
            ::boost::shared_ptr<
              detail::wrap2<detail::digit_array<unsigned long>, detail::lowlevel>
            >
          >
        > natural_number;

} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_NATURAL_NUMBER_HPP
