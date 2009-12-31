/*
 * File:   numbers/detail/natural_number_base.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-29 12:56Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_BASE_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_BASE_HPP_

#include <sputsoft/types.hpp>
#include <boost/integer_traits.hpp>
#include <sputsoft/number_theory/common.hpp>
#include <sputsoft/numbers/detail/expressions.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename ContArith>
class natural_number_base {
private:
  ContArith cont_arith;

  typedef typename ContArith::container_type container_type;

  container_type& get() { return cont_arith.get_con(); }
  const container_type& get() const { return cont_arith.get_con(); }

  static inline void set(natural_number_base& r, const natural_number_base& u) {
    ContArith::set(r.get(), u.get());
  }

  static inline void add(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    ContArith::add(r.get(), u.get(), v.get());
  }

  static inline void sub(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    ContArith::subtract(r.get(), u.get(), v.get());
  }

  static inline void multiply(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    ContArith::multiply(r.get(), u.get(), v.get());
  }

  static inline void divide(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    ContArith::divide(r.get(), u.get(), v.get());
  }

  static inline void remainder(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    ContArith::remainder(r.get(), u.get(), v.get());
  }

#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(type) \
  static inline void add(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    ContArith::add(r.get(), u.get(), to_unsigned(v)); \
  } \
  static inline void add(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    ContArith::add(r.get(), v.get(), to_unsigned(u)); \
  } \
  static inline void subtract(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    ContArith::subtract(r.get(), u.get(), to_unsigned(v)); \
  } \
  static inline void subtract(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    assign(ContArith::subtract(to_unsigned(u), v.get())); \
  } \
  static inline void multiply(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    ContArith::multiply(r.get(), u.get(), to_unsigned(v)); \
  } \
  static inline void multiply(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    ContArith::multiply(r.get(), v.get(), to_unsigned(u)); \
  } \
  static inline void divide(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    ContArith::divide(r.get(), u.get(), to_unsigned(v)); \
  } \
  static inline void divide(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    assign(divide(to_unsigned(u), v.get())); \
  } \
  static inline void remainder(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    assign(remainder(u.get(), to_unsigned(v))); \
  } \
  static inline void remainder(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    ContArith::remainder(r.get(), to_unsigned(u), v.get()); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(unsigned long)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(signed short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(signed)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(signed long)

  static inline void left_shift(natural_number_base& r,
      const natural_number_base& u, std::size_t count) {
    ContArith::left_shift(r.get(), u.get(), count);
  }

  static inline void right_shift(natural_number_base& r,
      const natural_number_base& u, std::size_t count) {
    ContArith::right_shift(r.get(), u.get(), count);
  }

public:

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::add,
      expr<R1, E1>, expr<R2, E2> >& e) {
    add(*this, e.x.eval(), e.y.eval());
  }

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::subtract,
      expr<R1, E1>, expr<R2, E2> >& e) {
    subtract(*this, e.x.eval(), e.y.eval());
  }

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::multiply,
      expr<R1, E1>, expr<R2, E2> >& e) {
    multiply(*this, e.x.eval(), e.y.eval());
  }

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::divide,
      expr<R1, E1>, expr<R2, E2> >& e) {
    divide(*this, e.x.eval(), e.y.eval());
  }

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::remainder,
      expr<R1, E1>, expr<R2, E2> >& e) {
    remainder(*this, e.x.eval(), e.y.eval());
  }

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::lshift,
      expr<R1, E1>, expr<R2, E2> >& e) {
    left_shift(*this, e.x.eval(), e.y.eval());
  }

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::rshift,
      expr<R1, E1>, expr<R2, E2> >& e) {
    right_shift(*this, e.x.eval(), e.y.eval());
  }

  inline void assign_expr(const natural_number_base& u) {
    set(*this, u);
  }

  natural_number_base() : cont_arith() {}

  template <typename E>
  natural_number_base(const E& e) : cont_arith() {
    assign_expr(e);
  }

  template <typename R, typename E>
  inline void add_this(const expr<R, E>& e) {
    add(*this, *this, e.eval());
  }

  template <typename T>
  inline void add_this(const T& v) {
    add(*this, *this, v);
  }

  template <typename R, typename E>
  inline void subtract_this(const expr<R, E>& e) {
    add(*this, *this, e.eval());
  }

  template <typename T>
  inline void subtract_this(const T& v) {
    add(*this, *this, v);
  }

  template <typename R, typename E>
  inline void divide_this(const expr<R, E>& e) {
    divide(*this, *this, e.eval());
  }

  template <typename T>
  inline void divide_this(const T& v) {
    divide(*this, *this, v);
  }

  template <typename R, typename E>
  inline void remainder_this(const expr<R, E>& e) {
    remainder(*this, *this, e.eval());
  }

  template <typename T>
  inline void remainder_this(const T& v) {
    remainder(*this, *this, v);
  }

  inline void left_shift_this(std::size_t count) {
    left_shift(*this, *this, count);
  }

  inline void right_shift_this(std::size_t count) {
    right_shift(*this, *this, count);
  }

  inline std::string to_string(unsigned base) const {
    return ContArith::to_string(get(), base);
  }

  inline bool is_zero() const {
    return ContArith::is_zero(get());
  }

  static inline std::pair<natural_number_base, natural_number_base>
        quotrem(const natural_number_base& x, const natural_number_base& y) {
    natural_number_base q, r;
    ContArith::quotrem(q.get(), r.get(), x.get(), y.get());
    return std::make_pair(q, r);
  }

  const typename ContArith::container_type& get_container() const {
    return get();
  }

#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(type) \
  natural_number_base(type u) : cont_arith() { \
    assign(u); \
  } \
  inline void assign(type u) { \
    ContArith::set(get(), to_unsigned(u)); \
  } \
  static inline type subtract(type x, const natural_number_base& y) { \
    return ContArith::subtract(to_unsigned(x), y.get()); \
  } \
  static inline type remainder(const natural_number_base& x, type y) { \
    return ContArith::remainder(x.get(), to_unsigned(y)); \
  } \
  static inline type divide(type x, const natural_number_base& y) { \
    return ContArith::divide(to_unsigned(x), y.get()); \
  } \
  static inline std::pair<natural_number_base, type> \
        quotrem(const natural_number_base& x, type y) { \
    natural_number_base q; \
    type r = ContArith::quotrem(q.get(), x.get(), to_unsigned(y)); \
    return std::make_pair(q, r); \
  } \
  static inline std::pair<type, natural_number_base> \
        quotrem(type x, const natural_number_base& y) { \
    natural_number_base r; \
    type q = ContArith::quotrem(r.get(), to_unsigned(x), y.get()); \
    return std::make_pair(q, r); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(unsigned long)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(signed short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(signed)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(signed long)

};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_BASE_HPP_
