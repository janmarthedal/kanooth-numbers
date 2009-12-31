/*
 * File:   numbers/detail/cont_arith_shared.hpp
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

#ifndef _SPUTSOFT_NUMBERS_DETAIL_CONT_ARITH_SHARED_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_CONT_ARITH_SHARED_HPP_

#include <boost/shared_ptr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename ContArith>
class cont_arith_shared {
public:
  typedef boost::shared_ptr<ContArith> container_type;

private:
  container_type con;

public:

  cont_arith_shared() : con(new ContArith) {}

  container_type& get_con() { return con; }
  const container_type& get_con() const { return con; }

  static inline bool is_zero(const container_type& u) {
    return ContArith::is_zero(u->get_con());
  }

  static void set(container_type& r, const container_type& x) {
    const typename ContArith::container_type& xc = x->get_con();
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::set(r->get_con(), xc);
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
    const typename ContArith::container_type& uc = u->get_con();
    if (!z.unique()) z = container_type(new ContArith);
    ContArith::left_shift(z->get_con(), uc, count);
  }

  static void right_shift(container_type& z, const container_type& u, std::size_t count) {
    const typename ContArith::container_type& uc = u->get_con();
    if (!z.unique()) z = container_type(new ContArith);
    ContArith::right_shift(z->get_con(), uc, count);
  }

  static inline void set(container_type& r, unsigned short u) {
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::set(r->get_con(), u);
  }
  static inline void add(container_type& r, const container_type& u, unsigned short v) {
    const typename ContArith::container_type& uc = u->get_con();
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::add(r->get_con(), u->get_con(), v);
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
    const typename ContArith::container_type& uc = u->get_con();
    if (!q.unique()) q = container_type(new ContArith);
    return ContArith::quotrem(q->get_con(), uc, v);
  }
  static inline unsigned short quotrem(container_type& r, unsigned short u, const container_type& v) {
  }

  static inline void set(container_type& r, unsigned u) {
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::set(r->get_con(), u);
  }
  static inline void add(container_type& r, const container_type& u, unsigned v) {
    const typename ContArith::container_type& uc = u->get_con();
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::add(r->get_con(), u->get_con(), v);
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
    const typename ContArith::container_type& uc = u->get_con();
    if (!q.unique()) q = container_type(new ContArith);
    return ContArith::quotrem(q->get_con(), uc, v);
  }
  static inline unsigned quotrem(container_type& r, unsigned u, const container_type& v) {
  }

  static inline void set(container_type& r, unsigned long u) {
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::set(r->get_con(), u);
  }
  static inline void add(container_type& r, const container_type& u, unsigned long v) {
    const typename ContArith::container_type& uc = u->get_con();
    if (!r.unique()) r = container_type(new ContArith);
    ContArith::add(r->get_con(), u->get_con(), v);
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
    const typename ContArith::container_type& uc = u->get_con();
    if (!q.unique()) q = container_type(new ContArith);
    return ContArith::quotrem(q->get_con(), uc, v);
  }
  static inline unsigned long quotrem(container_type& r, unsigned long u, const container_type& v) {
  }

};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_CONT_ARITH_HPP_
