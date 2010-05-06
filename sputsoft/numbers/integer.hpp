/* 
 * File:   integer.hpp
 * Author: jmr
 *
 * Created on May 6, 2010, 10:11 AM
 */

#ifndef _INTEGER_HPP
#define	_INTEGER_HPP

#include <sputsoft/numbers/natural_number.hpp>
#include <sputsoft/numbers/detail/int_impl.hpp>

namespace sputsoft {
namespace numbers {

typedef detail::expr<detail::intnum<natural_number> > integer;

} // namespace sputsoft
} // namespace numbers


#endif	/* _INTEGER_HPP */

