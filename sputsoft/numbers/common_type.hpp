/*
 * File:   sputsoft/numbers/common_type.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-09-10 13:27Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_COMMON_TYPE_HPP
#define _SPUTSOFT_NUMBERS_COMMON_TYPE_HPP

#include <sputsoft/type_traits.hpp>

namespace sputsoft {
namespace numbers {

namespace detail {

template <typename T> struct type_rank;
template <> struct type_rank<unsigned short>     { static const int value = 1; };
template <> struct type_rank<signed short>       { static const int value = 2; };
template <> struct type_rank<unsigned int>       { static const int value = 3; };
template <> struct type_rank<signed int>         { static const int value = 4; };
template <> struct type_rank<unsigned long>      { static const int value = 5; };
template <> struct type_rank<signed long>        { static const int value = 6; };
#ifdef SPUTSOFT_HAS_LONG_LONG
template <> struct type_rank<unsigned long long> { static const int value = 7; };
template <> struct type_rank<signed long long>   { static const int value = 8; };
#endif

template <bool C, typename T, typename F>
struct choose_type { typedef F type; };
template <typename T, typename F>
struct choose_type<true, T, F> { typedef T type; };

}

template <typename T1, typename T2>
struct common_type {
private:
  typedef typename detail::choose_type<detail::type_rank<T1>::value >= detail::type_rank<T2>::value,
          T1, T2>::type type__;
public:
  typedef typename detail::choose_type<
            sputsoft::is_signed<T1>::value || sputsoft::is_signed<T2>::value,
            typename sputsoft::make_signed<type__>::type,
            type__
          >::type type;
};


} // numbers
} // sputsoft

#endif // _SPUTSOFT_NUMBERS_COMMON_TYPE_HPP
