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

#ifndef _SPUTSOFT_TYPE_TRAITS_HPP_
#define _SPUTSOFT_TYPE_TRAITS_HPP_

#include <boost/type_traits.hpp>

#ifdef BOOST_HAS_LONG_LONG
#define SPUTSOFT_HAS_LONG_LONG
#endif

namespace sputsoft {

  template <typename T>
  struct make_signed : boost::make_signed<T> {};

  template <typename T>
  struct make_unsigned : boost::make_unsigned<T> {};

  template <typename T>
  inline typename make_unsigned<T>::type to_unsigned(const T& v) {
    return (typename make_unsigned<T>::type) v;
  }

} // namespace sputsoft

#endif // _SPUTSOFT_TYPE_TRAITS_HPP_
