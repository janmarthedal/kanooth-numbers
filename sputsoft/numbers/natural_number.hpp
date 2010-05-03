/* 
 * File:   natural_number.hpp
 * Author: jmr
 *
 * Created on May 3, 2010, 10:12 PM
 *
 * $Id$
 */

#ifndef _NATURAL_NUMBER_HPP
#define	_NATURAL_NUMBER_HPP

#include <sputsoft/numbers/detail/digit_array.hpp>
#include <sputsoft/numbers/detail/array_allocator.hpp>
#include <sputsoft/numbers/detail/nat_num_shared.hpp>
//#include <sputsoft/numbers/detail/nat_num_impl.hpp>

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

typedef detail::expr<
          detail::natnum<
            ::boost::shared_ptr<
              detail::wrap2<detail::digit_array<unsigned long>, detail::lowlevel>
            >
          >
        > natural_number;


/*typedef detail::expr<
          detail::natnum<
            detail::wrap2<detail::digit_array<unsigned long>, detail::lowlevel>
          >
        > natural_number;*/

} // namespace sputsoft
} // namespace numbers

#endif	/* _NATURAL_NUMBER_HPP */

