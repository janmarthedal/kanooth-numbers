#ifndef KANOOTH_CONFIG_HPP
#define KANOOTH_CONFIG_HPP

#ifdef KANOOTH_USE_BOOST

#include <boost/config.hpp>

#ifdef BOOST_HAS_LONG_LONG
#define KANOOTH_HAS_LONG_LONG
#endif

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
#  define KANOOTH_NO_RVALUE_REFS
#endif

#else // KANOOTH_USE_BOOST

#include <limits>

#ifdef __GNUC__
#define KANOOTH_HAS_LONG_LONG
#endif // __GNUC__

// http://www.stroustrup.com/C++11FAQ.html#11
#if __cplusplus > 199711L

// from boost/config/compiler/gcc.hpp:
#elif (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 2)) && defined(__GXX_EXPERIMENTAL_CXX0X__)

#else
#  define KANOOTH_NO_RVALUE_REFS
#endif


#endif // KANOOTH_USE_BOOST

#endif // KANOOTH_CONFIG_HPP
