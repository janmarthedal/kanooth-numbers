/*
 * File:   types.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-19 09:18Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id: types.hpp 54 2009-12-31 12:03:06Z jan_marthedal $
 */

#ifndef _SPUTSOFT_TYPES_HPP_
#define _SPUTSOFT_TYPES_HPP_

#include <boost/integer_traits.hpp>

namespace sputsoft {

  template <typename T> struct type_convert;

  template <>
  struct type_convert<unsigned short int> {
    typedef unsigned short int unsigned_type;
  };
  template <>
  struct type_convert<signed short int> {
    typedef unsigned short int unsigned_type;
  };
  template <>
  struct type_convert<unsigned int> {
    typedef unsigned int unsigned_type;
  };
  template <>
  struct type_convert<signed int> {
    typedef unsigned int unsigned_type;
  };
  template <>
  struct type_convert<unsigned long int> {
    typedef unsigned long int unsigned_type;
  };
  template <>
  struct type_convert<signed long int> {
    typedef unsigned long int unsigned_type;
  };

  template <typename T>
  typename type_convert<T>::unsigned_type to_unsigned(const T& v) {
    return (typename type_convert<T>::unsigned_type) v;
  }

#ifndef BOOST_HAS_LONG_LONG
#define SPUTSOFT_HAS_LONG_LONG
#endif

} // namespace sputsoft

#endif // _SPUTSOFT_TYPES_HPP_
