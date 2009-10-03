/*
 * File:   rational_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-10-03 15:30Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _RATIONAL_NUMBER_HPP
#define _RATIONAL_NUMBER_HPP

#ifdef NDEBUG
#define BOOST_DISABLE_ASSERTS
#endif

#include <natural_number_algorithms.hpp>

namespace sputsoft {
namespace multiprecision {


template <typename INT, typename NAT>
class rational_number {
private:
  INT num;
  NAT den;
public:
  rational_number(const INT& n);
  rational_number(const INT& n, const NAT& d);
}

template <typename INT, typename NAT>
rational_number<INT,NAT>::rational_number(const INT& n) : num(n), den(1)
{}

template <typename INT, typename NAT>
rational_number<INT,NAT>::rational_number(const INT& n, const NAT& d) : num(n), den(d)
{
  NAT c = gcd(abs(n), d);
  num = n/c;
  den = d/c;
}


} // multiprecision
} // sputsoft


#endif	/* _RATIONAL_NUMBER_HPP */
