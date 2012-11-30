#ifndef KANOOTH_IS_SIGNED_HPP
#define KANOOTH_IS_SIGNED_HPP

namespace kanooth {

#ifdef KANOOTH_USE_BOOST

  template <typename T> struct is_signed : public boost::is_signed<T> {};

#else

  template <typename T>
  struct is_signed {
      static const bool value = std::numeric_limits<T>::is_signed;
  };

#endif

} // namespace kanooth

#endif // KANOOTH_IS_SIGNED_HPP
