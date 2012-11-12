/*
 * File:   kanooth/number_bits.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-19 09:18Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2012
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_TYPE_TRAITS_HPP_
#define _KANOOTH_TYPE_TRAITS_HPP_

#ifdef KANOOTH_USE_BOOST
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

#ifdef KANOOTH_USE_BOOST

  template <typename T>
  struct number_bits {
      static const unsigned value = boost::integer_traits<T>::digits;
  };

#else

  template <typename T>
  struct number_bits {
      static const unsigned value = std::numeric_limits<T>::digits;
  };

#endif

} // namespace kanooth

#endif // _KANOOTH_TYPE_TRAITS_HPP_
