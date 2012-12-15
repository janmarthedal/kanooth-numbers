#ifndef KANOOTH_MAKE_SIGNED_HPP
#define KANOOTH_MAKE_SIGNED_HPP

#include <kanooth/config.hpp>
#include <kanooth/detail/set_type.hpp>
#include <kanooth/fixed_width_ints.hpp>

namespace kanooth {

#ifdef KANOOTH_USE_BOOST

#include <boost/type_traits/make_signed.hpp>
template <typename T> struct make_signed : boost::make_signed<T> {};

#else

  template <typename T> struct make_signed {};
  template <> struct make_signed<unsigned char> : public detail::set_type<signed char> {};
  template <> struct make_signed<signed char> : public detail::set_type<signed char> {};
  template <> struct make_signed<unsigned short> : public detail::set_type<signed short> {};
  template <> struct make_signed<signed short> : public detail::set_type<signed short> {};
  template <> struct make_signed<unsigned int> : public detail::set_type<signed int> {};
  template <> struct make_signed<signed int> : public detail::set_type<signed int> {};
  template <> struct make_signed<unsigned long> : public detail::set_type<signed long> {};
  template <> struct make_signed<signed long> : public detail::set_type<signed long> {};
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct make_signed<unsigned long long> : public detail::set_type<signed long long> {};
  template <> struct make_signed<signed long long> : public detail::set_type<signed long long> {};
#endif
#ifdef KANOOTH_HAS_INT128_T
  template <> struct make_signed<uint128_t> : public detail::set_type<int128_t> {};
  template <> struct make_signed<int128_t> : public detail::set_type<int128_t> {};
#endif

#endif

} // namespace kanooth

#endif // KANOOTH_MAKE_SIGNED_HPP
