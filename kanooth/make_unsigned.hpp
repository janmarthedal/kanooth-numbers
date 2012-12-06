#ifndef KANOOTH_MAKE_UNSIGNED_HPP
#define KANOOTH_MAKE_UNSIGNED_HPP

#include <kanooth/config.hpp>
#include <kanooth/detail/set_type.hpp>

namespace kanooth {

#ifdef KANOOTH_USE_BOOST

  template <typename T> struct make_unsigned : boost::make_unsigned<T> {};

#else

  template <typename T> struct make_unsigned {};
  template <> struct make_unsigned<unsigned short> : public detail::set_type<unsigned short> {};
  template <> struct make_unsigned<signed short> : public detail::set_type<unsigned short> {};
  template <> struct make_unsigned<unsigned int> : public detail::set_type<unsigned int> {};
  template <> struct make_unsigned<signed int> : public detail::set_type<unsigned int> {};
  template <> struct make_unsigned<unsigned long> : public detail::set_type<unsigned long> {};
  template <> struct make_unsigned<signed long> : public detail::set_type<unsigned long> {};
#ifdef KANOOTH_HAS_LONG_LONG
  template <> struct make_unsigned<unsigned long long> : public detail::set_type<unsigned long long> {};
  template <> struct make_unsigned<signed long long> : public detail::set_type<unsigned long long> {};
#endif

#endif

} // namespace kanooth

#endif // KANOOTH_MAKE_UNSIGNED_HPP
