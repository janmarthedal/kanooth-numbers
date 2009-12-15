/*
 * File:   math/numbers/generic/natural_number.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_MATH_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_MATH_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_

#include <iostream>
#include <sputsoft/math/numbers/detail/expressions.hpp>

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace generic {

template <typename Con, typename LowLevel>
class midlevel {

  static void add3(Con& r, const Con& x, const Con& y) {
    std::size_t xs = x.size();
    typename Con::digit_type carry =
      LowLevel::add(r.get(), x.get(), xs, y.get(), y.size());
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= y.size() >= 1
  static void add2(Con& r, const Con& x, const Con& y) {
    std::size_t xs = x.size();
    if (!r.will_fit(xs + 1)) {
      Con t(xs + 1);
      add3(t, x, y);
      r.swap(t);
    } else 
      add3(r, x, y);
  }

  static void add3(Con& r, const Con& x, unsigned long y) {
    std::size_t xs = x.size();
    typename Con::digit_type carry =
      LowLevel::add_1(r.get(), x.get(), xs, y);
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= 1, y != 0
  static void add2(Con& r, const Con& x, unsigned long y) {
    std::size_t xs = x.size();
    if (!r.will_fit(xs + 1)) {
      Con t(xs + 1);
      add3(t, x, y);
      r.swap(t);
    } else 
      add3(r, x, y);    
  }

public:
  
  typedef Con container_type;

  static void set(Con& r, const unsigned long x) {
    if (x) {
      if (!r.will_fit(1)) Con(1).swap(r);
      r[0] = x;
      r.set_size(1);
    } else
      r.set_size(0);
  }

  static void set(Con& r, const Con& x) {
    LowLevel::copy(r.get(), x.get(), x.size());
    r.set_size(x.size());
  }

  static void add(Con& r, const Con& x, const Con& y) {
    if (y.is_empty())             set(r, x);
    else if (x.is_empty())        set(r, y);
    else if (x.size() > y.size()) add2(r, x, y);
    else                          add2(r, y, x);
  }

  static void add(Con& r, const Con& x, unsigned long y) {
    if (x.is_empty()) set(r, y);
    else if (!y)      set(r, x);
    else              add2(r, x, y);
  }

};

using namespace ::detail::expr;
using namespace ::detail::ops::binary;
using namespace ::detail::ops::unary;

template <typename MidLevel>
class natnum {
  template <typename ML>
  friend std::ostream& operator<<(std::ostream& os, const natnum<ML>& n);
private:
  typename MidLevel::container_type digits;
public:
  natnum() : digits() {}
  template <typename E>
  natnum(const E& e) : digits() { assign(*this, e); }

  static inline void assign(natnum& r, const binary<add, natnum, natnum>& expr) {
    MidLevel::add(r.digits, expr.x.digits, expr.y.digits);
  }

  static inline void assign(natnum& r, const binary<add, natnum, unsigned long>& expr) {
    MidLevel::add(r.digits, expr.x.digits, expr.y);
  }

  static inline void assign(natnum& r, const binary<add, natnum, unsigned>& expr) {
    MidLevel::add(r.digits, expr.x.digits, expr.y);
  }

  static inline void assign(natnum& r, const binary<add, natnum, unsigned short>& expr) {
    MidLevel::add(r.digits, expr.x.digits, expr.y);
  }

  static inline void assign(natnum& r, const binary<add, unsigned long, natnum>& expr) {
    MidLevel::add(r.digits, expr.y.digits, expr.x);
  }

  static inline void assign(natnum& r, const binary<add, unsigned, natnum>& expr) {
    MidLevel::add(r.digits, expr.y.digits, expr.x);
  }

  static inline void assign(natnum& r, const binary<add, unsigned short, natnum>& expr) {
    MidLevel::add(r.digits, expr.y.digits, expr.x);
  }

  template <typename X>
  static inline void assign(natnum& r, const binary<add, X, unsigned long>& expr) {
    MidLevel::add(r.digits, natnum(expr.x).digits, expr.y);
  }

  template <typename X>
  static inline void assign(natnum& r, const binary<add, X, unsigned>& expr) {
    MidLevel::add(r.digits, natnum(expr.x).digits, expr.y);
  }

  template <typename X>
  static inline void assign(natnum& r, const binary<add, X, unsigned short>& expr) {
    MidLevel::add(r.digits, natnum(expr.x).digits, expr.y);
  }

  template <typename Y>
  static inline void assign(natnum& r, const binary<add, unsigned short, Y>& expr) {
    MidLevel::add(r.digits, natnum(expr.y).digits, expr.x);
  }

  template <typename Y>
  static inline void assign(natnum& r, const binary<add, unsigned, Y>& expr) {
    MidLevel::add(r.digits, natnum(expr.y).digits, expr.x);
  }

  template <typename Y>
  static inline void assign(natnum& r, const binary<add, unsigned long, Y>& expr) {
    MidLevel::add(r.digits, natnum(expr.y).digits, expr.x);
  }

  template <typename Y>
  static inline void assign(natnum& r, const binary<add, natnum, Y>& expr) {
    MidLevel::add(r.digits, expr.x.digits, natnum(expr.y).digits);
  }

  template <typename X>
  static inline void assign(natnum& r, const binary<add, X, natnum>& expr) {
    MidLevel::add(r.digits, natnum(expr.x).digits, expr.y.digits);
  }

  template <typename X, typename Y>
  static inline void assign(natnum& r, const binary<add, X, Y>& expr) {
    MidLevel::add(r.digits, natnum(expr.x).digits, natnum(expr.y).digits);
  }

  static inline void assign(natnum& r, unsigned long x) {
    MidLevel::set(r.digits, x);
  }

  static inline void assign(natnum& r, unsigned x) {
    MidLevel::set(r.digits, x);
  }

  static inline void assign(natnum& r, unsigned short x) {
    MidLevel::set(r.digits, x);
  }

};

template <typename ML>
std::ostream& operator<<(std::ostream& os, const natnum<ML>& n)
{
  return os << n.digits;
}

} // namespace generic
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_GENERIC_NATURAL_NUMBER_HPP_

