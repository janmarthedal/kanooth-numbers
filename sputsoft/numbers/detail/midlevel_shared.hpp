/*
 * File:   numbers/detail/midlevel_shared.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-29 21:33Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_MIDLEVEL_SHARED_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_MIDLEVEL_SHARED_HPP_

#include <boost/shared_ptr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename MidLevel>
class midlevel_shared {

  typedef typename MidLevel::container_type Con2;

public:

  typedef boost::shared_ptr<Con2> container_type;

  static void set(container_type& r, const container_type& x) {
    if (r.get() && r.unique())
      MidLevel::set(*r, *x);
    else {
      container_type t(new Con2);
      MidLevel::set(*t, *x);
      r.swap(t);
    }
  }

  static void add(container_type& r, const container_type& x, const container_type& y) {
  }

  static void subtract(container_type& r, const container_type& x, const container_type& y) {
  }

  static void multiply(container_type& r, const container_type& x, const container_type& y) {
  }

  static void divide(container_type& q, const container_type& u, const container_type& v) {
  }

  static void remainder(container_type& r, const container_type& u, const container_type& v) {
  }

  static void quotrem(container_type& q, container_type& r, const container_type& u, const container_type& v) {
  }

  static void left_shift(container_type& z, const container_type& u, std::size_t count) {
  }

  static void right_shift(container_type& z, const container_type& u, std::size_t count) {
  }

  static inline void set(container_type& r, unsigned short u) {
  }
  static inline void add(container_type& r, const container_type& u, unsigned short v) {
  }
  static inline void subtract(container_type& r, const container_type& u, unsigned short v) {
  }
  static inline unsigned short subtract(unsigned short u, const container_type& v) {
  }
  static inline void multiply(container_type& r, const container_type& u, unsigned short v) {
  }
  static inline void divide(container_type& r, const container_type& u, unsigned short v) {
  }
  static inline unsigned short divide(unsigned short u, const container_type& v) {
  }
  static inline unsigned short remainder(const container_type& u, unsigned short v) {
  }
  static inline void remainder(container_type& r, unsigned short u, const container_type& v) {
  }
  static inline unsigned short quotrem(container_type& q, const container_type& u, unsigned short v) {
  }
  static inline unsigned short quotrem(container_type& r, unsigned short u, const container_type& v) {
  }

  static inline void set(container_type& r, unsigned u) {
  }
  static inline void add(container_type& r, const container_type& u, unsigned v) {
  }
  static inline void subtract(container_type& r, const container_type& u, unsigned v) {
  }
  static inline unsigned subtract(unsigned u, const container_type& v) {
  }
  static inline void multiply(container_type& r, const container_type& u, unsigned v) {
  }
  static inline void divide(container_type& r, const container_type& u, unsigned v) {
  }
  static inline unsigned divide(unsigned u, const container_type& v) {
  }
  static inline unsigned remainder(const container_type& u, unsigned v) {
  }
  static inline void remainder(container_type& r, unsigned u, const container_type& v) {
  }
  static inline unsigned quotrem(container_type& q, const container_type& u, unsigned v) {
  }
  static inline unsigned quotrem(container_type& r, unsigned u, const container_type& v) {
  }

  static inline void set(container_type& r, unsigned long u) {
  }
  static inline void add(container_type& r, const container_type& u, unsigned long v) {
  }
  static inline void subtract(container_type& r, const container_type& u, unsigned long v) {
  }
  static inline unsigned long subtract(unsigned long u, const container_type& v) {
  }
  static inline void multiply(container_type& r, const container_type& u, unsigned long v) {
  }
  static inline void divide(container_type& r, const container_type& u, unsigned long v) {
  }
  static inline unsigned long divide(unsigned long u, const container_type& v) {
  }
  static inline unsigned long remainder(const container_type& u, unsigned long v) {
  }
  static inline void remainder(container_type& r, unsigned long u, const container_type& v) {
  }
  static inline unsigned long quotrem(container_type& q, const container_type& u, unsigned long v) {
  }
  static inline unsigned long quotrem(container_type& r, unsigned long u, const container_type& v) {
  }

};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_MIDLEVEL_HPP_
