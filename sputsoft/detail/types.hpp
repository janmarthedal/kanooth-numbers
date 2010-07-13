/*
 * File:   sputsoft/detail/types.hpp
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

#ifndef _SPUTSOFT_DETAIL_TYPES_HPP_
#define _SPUTSOFT_DETAIL_TYPES_HPP_

#include <boost/integer_traits.hpp>

#ifdef BOOST_HAS_LONG_LONG
#define SPUTSOFT_HAS_LONG_LONG
#endif

namespace sputsoft {

  template <typename T> struct number_traits;

  template <>
  struct number_traits<unsigned short> {
    typedef unsigned short unsigned_type;
    typedef signed short signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = false);
  };

  template <>
  struct number_traits<signed short> {
    typedef unsigned short unsigned_type;
    typedef signed short signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = true);
  };

  template <>
  struct number_traits<unsigned> {
    typedef unsigned unsigned_type;
    typedef signed signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = false);
  };

  template <>
  struct number_traits<signed> {
    typedef unsigned unsigned_type;
    typedef signed signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = true);
  };

  template <>
  struct number_traits<unsigned long> {
    typedef unsigned long unsigned_type;
    typedef signed long signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = false);
  };

  template <>
  struct number_traits<signed long> {
    typedef unsigned long unsigned_type;
    typedef signed long signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = true);
  };

#ifdef SPUTSOFT_HAS_LONG_LONG
  template <>
  struct number_traits<unsigned long long> {
    typedef unsigned long long unsigned_type;
    typedef signed long long signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = false);
  };

  template <>
  struct number_traits<signed long long> {
    typedef unsigned long long unsigned_type;
    typedef signed long long signed_type;
    BOOST_STATIC_CONSTANT(bool, is_native = true);
    BOOST_STATIC_CONSTANT(bool, is_signed = true);
  };
#endif

  template <typename T>
  inline typename number_traits<T>::unsigned_type to_unsigned(const T& v) {
    return (typename number_traits<T>::unsigned_type) v;
  }

} // namespace sputsoft

#endif // _SPUTSOFT_DETAIL_TYPES_HPP_
