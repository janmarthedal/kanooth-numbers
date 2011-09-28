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

  struct no_type {
    static const bool enabled = false;
  };

  template <typename T>
  struct set_type {
    static const bool enabled = true;
    typedef T type;
  };

  template <bool C, typename T>
  struct type_if : public no_type {};

  template <typename T>
  struct type_if<true, T> : public set_type<T> {};

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

  template <typename T> struct make_signed : public no_type {};
  template <> struct make_signed<unsigned short> : public set_type<signed short> {};
  template <> struct make_signed<signed short> : public set_type<signed short> {};
  template <> struct make_signed<unsigned int> : public set_type<signed int> {};
  template <> struct make_signed<signed int> : public set_type<signed int> {};
  template <> struct make_signed<unsigned long> : public set_type<signed long> {};
  template <> struct make_signed<signed long> : public set_type<signed long> {};
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct make_signed<unsigned long long> : public set_type<signed long long> {};
  template <> struct make_signed<signed long long> : public set_type<signed long long> {};
#endif
  template <> struct make_signed<float> : public set_type<float> {};
  template <> struct make_signed<double> : public set_type<double> {};
  template <> struct make_signed<long double> : public set_type<long double> {};

  template <typename T> struct make_unsigned {};
  template <> struct make_unsigned<unsigned short> : public set_type<unsigned short> {};
  template <> struct make_unsigned<signed short> : public set_type<unsigned short> {};
  template <> struct make_unsigned<unsigned int> : public set_type<unsigned int> {};
  template <> struct make_unsigned<signed int> : public set_type<unsigned int> {};
  template <> struct make_unsigned<unsigned long> : public set_type<unsigned long> {};
  template <> struct make_unsigned<signed long> : public set_type<unsigned long> {};
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct make_unsigned<unsigned long long> : public set_type<unsigned long long> {};
  template <> struct make_unsigned<signed long long> : public set_type<unsigned long long> {};
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

template <typename T> struct is_native_float : public false_type {};
template <> struct is_native_float<float>       : public true_type {};
template <> struct is_native_float<double>      : public true_type {};
template <> struct is_native_float<long double> : public true_type {};

template <typename T>
struct is_native_number
  : public integral_constant<bool, is_native_int<T>::value || is_native_float<T>::value> {};

template <bool C, typename T>
struct make_signed_if : public set_type<T> {};
template <typename T>
struct make_signed_if<true, T> : public make_signed<T> {};

template <bool C, typename T>
struct make_unsigned_if : public set_type<T> {};
template <typename T>
struct make_unsigned_if<true, T> : public make_unsigned<T> {};

} // namespace kanooth

#endif // _KANOOTH_TYPE_TRAITS_HPP_
