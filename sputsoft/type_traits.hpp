/*
 * File:   sputsoft/type_traits.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-19 09:18Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_TYPE_TRAITS_HPP_
#define _SPUTSOFT_TYPE_TRAITS_HPP_

#ifdef SPUTSOFT_USE_BOOST
#include <boost/type_traits.hpp>
#include <boost/integer_traits.hpp>
#ifdef BOOST_HAS_LONG_LONG
#define SPUTSOFT_HAS_LONG_LONG
#endif
#else
#include <limits>
#ifdef __GNUC__
#define SPUTSOFT_HAS_LONG_LONG
#endif
#endif

namespace sputsoft {

#ifdef SPUTSOFT_USE_BOOST

  template <typename T>
  struct make_signed : boost::make_signed<T> {};

  template <typename T>
  struct make_unsigned : boost::make_unsigned<T> {};

  template <typename T>
  struct number_bits {
    static const unsigned value = boost::integer_traits<T>::digits;
  };

#else

  template <typename T> struct make_signed;
  template <> struct make_signed<unsigned short> { typedef signed short type; };
  template <> struct make_signed<signed short> { typedef signed short type; };
  template <> struct make_signed<unsigned int> { typedef signed int type; };
  template <> struct make_signed<signed int> { typedef signed int type; };
  template <> struct make_signed<unsigned long> { typedef signed long type; };
  template <> struct make_signed<signed long> { typedef signed long type; };
#ifdef SPUTSOFT_HAS_LONG_LONG
  template <> struct make_signed<unsigned long long> { typedef signed long long type; };
  template <> struct make_signed<signed long long> { typedef signed long long type; };
#endif

  template <typename T> struct make_unsigned;
  template <> struct make_unsigned<unsigned short> { typedef unsigned short type; };
  template <> struct make_unsigned<signed short> { typedef unsigned short type; };
  template <> struct make_unsigned<unsigned int> { typedef unsigned int type; };
  template <> struct make_unsigned<signed int> { typedef unsigned int type; };
  template <> struct make_unsigned<unsigned long> { typedef unsigned long type; };
  template <> struct make_unsigned<signed long> { typedef unsigned long type; };
#ifdef SPUTSOFT_HAS_LONG_LONG
  template <> struct make_unsigned<unsigned long long> { typedef unsigned long long type; };
  template <> struct make_unsigned<signed long long> { typedef unsigned long long type; };
#endif
  
  template <typename T>
  struct number_bits {
    static const unsigned value = std::numeric_limits<T>::digits;
  };
  
#endif

  template <typename T>
  inline typename make_unsigned<T>::type to_unsigned(const T& v) {
    return (typename make_unsigned<T>::type) v;
  }

} // namespace sputsoft

#endif // _SPUTSOFT_TYPE_TRAITS_HPP_
