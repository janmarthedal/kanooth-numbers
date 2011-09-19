/*
 * File:   kanooth/numbers/natural_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-03 20:12Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
#define	_KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

#include <kanooth/numbers/detail/memory/digit_array.hpp>
#include <kanooth/numbers/detail/memory/array_allocator.hpp>
//#include <kanooth/numbers/detail/natural_number/nat_num_shared.hpp>
#include <kanooth/numbers/detail/natural_number/nat_num_impl.hpp>
#include <kanooth/numbers/detail/default_ops.hpp>
#include <kanooth/numbers/detail/overload.hpp>

#ifdef KANOOTH_USE_GMP
#include <kanooth/numbers/detail/lowlevel/gmp.hpp>
#else
#include <kanooth/numbers/detail/lowlevel/generic.hpp>
#endif

namespace kanooth {
namespace numbers {
namespace detail {

#ifdef KANOOTH_USE_GMP
typedef lowlevel_gmp lowlevel;
#else
typedef lowlevel_generic<array_allocator<void> > lowlevel;
#endif

} // namespace detail

typedef detail::numb<
          detail::natnum<
            //kanooth::shared_ptr<
              detail::wrap2<detail::digit_array<unsigned long>, detail::lowlevel>
            //>
          >
        > natural_number;

} // namespace kanooth
} // namespace numbers

#endif // _KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
