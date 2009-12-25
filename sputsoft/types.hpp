/*
 * File:   types.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-19 09:18Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_TYPES_HPP_
#define _SPUTSOFT_TYPES_HPP_

#include <boost/cstdint.hpp>

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

/*  using ::boost::int8_t;
  using ::boost::uint8_t;
  using ::boost::int16_t;
  using ::boost::uint16_t;
  using ::boost::int32_t;
  using ::boost::uint32_t;

#ifndef BOOST_NO_INT64_T

#define SPUTSOFT_HAS_64_BIT_TYPES
  using ::boost::int64_t;
  using ::boost::uint64_t;

#endif*/

} // namespace sputsoft

#endif // _SPUTSOFT_TYPES_HPP_

