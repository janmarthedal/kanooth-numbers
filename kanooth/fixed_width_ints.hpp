#ifndef KANOOTH_FIXED_WIDTH_INTS_HPP
#define KANOOTH_FIXED_WIDTH_INTS_HPP

#ifdef KANOOTH_USE_BOOST

#include <boost/cstdint.hpp>

namespace kanooth {

using ::boost::int8_t;
using ::boost::uint8_t;
using ::boost::int16_t;
using ::boost::uint16_t;
using ::boost::int32_t;
using ::boost::uint32_t;

#ifndef BOOST_NO_INT64_T
#define KANOOTH_HAS_INT64_T
using ::boost::int64_t;
using ::boost::uint64_t;
#endif    

#ifdef BOOST_HAS_INT128
#define KANOOTH_HAS_INT128_T
typedef ::boost::int128_type int128_t;
typedef ::boost::uint128_type uint128_t;
#endif

}

#else

#include <kanooth/config.hpp>
#include <stdint.h>

namespace kanooth {

typedef ::int8_t   int8_t;
typedef ::uint8_t  uint8_t;
typedef ::int16_t  int16_t;
typedef ::uint16_t uint16_t;
typedef ::int32_t  int32_t;
typedef ::uint32_t uint32_t;

#ifdef __GNUC__
#ifdef __x86_64__

#define KANOOTH_HAS_INT64_T
typedef ::int64_t  int64_t;
typedef ::uint64_t uint64_t;

#define KANOOTH_HAS_INT128_T
__extension__ typedef __int128 int128_t;
__extension__ typedef unsigned __int128 uint128_t;

#endif // __x86_64__
#endif // __GNUC__

} // namespace kanooth

#endif

#endif // KANOOTH_FIXED_WIDTH_INTS_HPP
