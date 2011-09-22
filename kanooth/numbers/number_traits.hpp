/*
 * File:   kanooth/numbers/number_traits.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-09-10 13:27Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_NUMBER_TRAITS_HPP
#define _KANOOTH_NUMBERS_NUMBER_TRAITS_HPP

#include <kanooth/type_traits.hpp>

namespace kanooth {
namespace numbers {

namespace detail {

template <typename T> struct type_rank;
template <> struct type_rank<unsigned short>     : integral_constant<int, 1> {};
template <> struct type_rank<signed short>       : integral_constant<int, 2> {};
template <> struct type_rank<unsigned int>       : integral_constant<int, 3> {};
template <> struct type_rank<signed int>         : integral_constant<int, 4> {};
template <> struct type_rank<unsigned long>      : integral_constant<int, 5> {};
template <> struct type_rank<signed long>        : integral_constant<int, 6> {};
#ifdef KANOOTH_HAS_LONG_LONG
template <> struct type_rank<unsigned long long> : integral_constant<int, 7> {};
template <> struct type_rank<signed long long>   : integral_constant<int, 8> {};
#endif
template <> struct type_rank<float>              : integral_constant<int, 9> {};
template <> struct type_rank<double>             : integral_constant<int, 10> {};
template <> struct type_rank<long double>        : integral_constant<int, 11> {};

template <bool C, typename T, typename F>
struct choose_type { typedef F type; };
template <typename T, typename F>
struct choose_type<true, T, F> { typedef T type; };

} // namespace details

// private helpers
namespace {

template <bool C, typename R, typename V>
struct is_assignable2 : public false_type {};

template <typename R, typename V>
struct is_assignable2<true, R, V>
  : public integral_constant<bool, (detail::type_rank<R>::value >= detail::type_rank<V>::value)
                                && (is_signed<R>::value || !is_signed<V>::value)> {};

}

template <typename T1, typename T2>
struct common_type
  : public kanooth::make_signed_if<kanooth::is_signed<T1>::value || kanooth::is_signed<T2>::value,
                 typename detail::choose_type<
                            detail::type_rank<T1>::value >= detail::type_rank<T2>::value, T1, T2
                                             >::type
                       > {};

template <typename T>
struct is_number : public integral_constant<bool, is_native_int<T>::value || is_native_float<T>::value> {};

template <typename R, typename V>
struct is_assignable
  : public is_assignable2<is_number<R>::value && is_number<V>::value, R, V> {};

} // numbers
} // kanooth

#endif // _KANOOTH_NUMBERS_NUMBER_TRAITS_HPP
