#ifndef KANOOTH_CONFIG_HPP
#define KANOOTH_CONFIG_HPP

#ifdef KANOOTH_USE_BOOST

#include <boost/config.hpp>

#ifdef BOOST_HAS_LONG_LONG
#define KANOOTH_HAS_LONG_LONG
#endif

#ifdef BOOST_HAS_INT64_T
#define KANOOTH_HAS_INT64_T
#endif

#else // KANOOTH_USE_BOOST

#include <limits>

#ifdef __GNUC__

#define KANOOTH_HAS_LONG_LONG

#endif // __GNUC__

#endif // KANOOTH_USE_BOOST

#endif // KANOOTH_CONFIG_HPP
