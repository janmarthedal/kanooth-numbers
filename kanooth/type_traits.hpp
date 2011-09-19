/*
 * File:   kanooth/type_traits.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-19 09:18Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_TYPE_TRAITS_HPP_
#define _KANOOTH_TYPE_TRAITS_HPP_

#ifdef KANOOTH_USE_BOOST
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer_traits.hpp>
#ifdef BOOST_HAS_LONG_LONG
#define KANOOTH_HAS_LONG_LONG
#endif
#else
#include <limits>
#ifdef __GNUC__
#define KANOOTH_HAS_LONG_LONG
#endif
#endif

namespace kanooth {

  template <bool C, typename T>
  struct type_if {
    static const bool enabled = false;
  };

  template <typename T>
  struct type_if<true, T> {
    static const bool enabled = true;
    typedef T type;
  };

  template <typename T, T val>
  struct integral_constant {
    static const T value = val;
  };

#ifdef KANOOTH_USE_BOOST

  template <typename T>
  struct number_bits {
    static const unsigned value = boost::integer_traits<T>::digits;
  };

  template <typename T> struct make_signed : boost::make_signed<T> {};
  template <typename T> struct make_unsigned : boost::make_unsigned<T> {};
  template <typename T> struct is_signed : public boost::is_signed<T> {};
  template <typename T> struct is_integral : public boost::is_integral<T> {};

  using boost::true_type;
  using boost::false_type;

#else

  typedef integral_constant<bool, true>  true_type;
  typedef integral_constant<bool, false> false_type;

  template <typename T> struct make_signed {};
  template <> struct make_signed<unsigned short> { typedef signed short type; };
  template <> struct make_signed<signed short> { typedef signed short type; };
  template <> struct make_signed<unsigned int> { typedef signed int type; };
  template <> struct make_signed<signed int> { typedef signed int type; };
  template <> struct make_signed<unsigned long> { typedef signed long type; };
  template <> struct make_signed<signed long> { typedef signed long type; };
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct make_signed<unsigned long long> { typedef signed long long type; };
  template <> struct make_signed<signed long long> { typedef signed long long type; };
#endif

  template <typename T> struct make_unsigned {};
  template <> struct make_unsigned<unsigned short> { typedef unsigned short type; };
  template <> struct make_unsigned<signed short> { typedef unsigned short type; };
  template <> struct make_unsigned<unsigned int> { typedef unsigned int type; };
  template <> struct make_unsigned<signed int> { typedef unsigned int type; };
  template <> struct make_unsigned<unsigned long> { typedef unsigned long type; };
  template <> struct make_unsigned<signed long> { typedef unsigned long type; };
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct make_unsigned<unsigned long long> { typedef unsigned long long type; };
  template <> struct make_unsigned<signed long long> { typedef unsigned long long type; };
#endif
  
  template <typename T>
  struct number_bits : public integral_constant<unsigned, std::numeric_limits<T>::digits> {};

  template <typename T>
  struct is_signed : public integral_constant <bool, std::numeric_limits<T>::is_signed> {};

  template <typename T> struct is_integral : public false_type {};
  template <> struct is_integral<unsigned short> : public true_type {};
  template <> struct is_integral<signed short> : public true_type {};
  template <> struct is_integral<unsigned int> : public true_type {};
  template <> struct is_integral<signed int> : public true_type {};
  template <> struct is_integral<unsigned long> : public true_type {};
  template <> struct is_integral<signed long> : public true_type {};
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct is_integral<unsigned long long> : public true_type {};
  template <> struct is_integral<signed long long> : public true_type {};
#endif

#endif

  template <typename T>
  inline typename make_unsigned<T>::type to_unsigned(const T& v) {
    return (typename make_unsigned<T>::type) v;
  }

  template <typename T> struct is_native_int : public false_type {};
  template <> struct is_native_int<unsigned short> : public true_type {};
  template <> struct is_native_int<signed short>   : public true_type {};
  template <> struct is_native_int<unsigned int>   : public true_type {};
  template <> struct is_native_int<signed int>     : public true_type {};
  template <> struct is_native_int<unsigned long>  : public true_type {};
  template <> struct is_native_int<signed long>    : public true_type {};
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct is_native_int<unsigned long long> : public true_type {};
  template <> struct is_native_int<signed long long>   : public true_type {};
#endif

template <bool C, typename T>
struct make_signed_if {
  typedef T type;
};
template <typename T>
struct make_signed_if<true, T> : public make_signed<T> {};

template <bool C, typename T>
struct make_unsigned_if {
  typedef T type;
};
template <typename T>
struct make_unsigned_if<true, T> : public make_unsigned<T> {};

} // namespace kanooth

#endif // _KANOOTH_TYPE_TRAITS_HPP_