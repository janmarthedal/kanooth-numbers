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

template <typename MidLevel>
class natural_number_base {
private:
  typename MidLevel::container_type digits;

  static inline void set(natural_number_base& r, const natural_number_base& u) {
    MidLevel::set(r.digits, u.digits);
  }

  static inline void add(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    MidLevel::add(r.digits, u.digits, v.digits);
  }

  static inline void sub(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    MidLevel::subtract(r.digits, u.digits, v.digits);
  }

  static inline void multiply(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    MidLevel::multiply(r.digits, u.digits, v.digits);
  }

  static inline void divide(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    MidLevel::divide(r.digits, u.digits, v.digits);
  }

  static inline void remainder(natural_number_base& r,
      const natural_number_base& u, const natural_number_base& v) {
    MidLevel::remainder(r.digits, u.digits, v.digits);
  }

#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(type) \
  static inline void add(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    MidLevel::add(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void add(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    MidLevel::add(r.digits, v.digits, (type_convert<type>::unsigned_type) u); \
  } \
  static inline void subtract(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    MidLevel::subtract(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void subtract(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    assign(MidLevel::subtract((type_convert<type>::unsigned_type) u, v.digits)); \
  } \
  static inline void multiply(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    MidLevel::multiply(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void multiply(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    MidLevel::multiply(r.digits, v.digits, (type_convert<type>::unsigned_type) u); \
  } \
  static inline void divide(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    MidLevel::divide(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void divide(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    assign(divide((type_convert<type>::unsigned_type) u, v.digits)); \
  } \
  static inline void remainder(natural_number_base& r, \
      const natural_number_base& u, type v) { \
    assign(remainder(u.digits, (type_convert<type>::unsigned_type) v)); \
  } \
  static inline void remainder(natural_number_base& r, \
      type u, const natural_number_base& v) { \
    MidLevel::remainder(r.digits, (type_convert<type>::unsigned_type) u, v.digits); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(unsigned long)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(signed short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(signed)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PRIVATE_BUILTIN(signed long)

  static inline void left_shift(natural_number_base& r,
      const natural_number_base& u, std::size_t count) {
    MidLevel::left_shift(r.digits, u.digits, count);
  }

  static inline void right_shift(natural_number_base& r,
      const natural_number_base& u, std::size_t count) {
    MidLevel::right_shift(r.digits, u.digits, count);
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

  natural_number_base() : digits() {}

  template <typename E>
  natural_number_base(const E& e) : digits() {
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

  inline std::string to_string(unsigned base) const {
    return MidLevel::to_string(digits, base);
  }

  inline bool is_zero() const {
    return digits.is_empty();
  }

  static inline std::pair<natural_number_base, natural_number_base>
        quotrem(const natural_number_base& x, const natural_number_base& y) {
    natural_number_base q, r;
    MidLevel::quotrem(q.digits, r.digits, x.digits, y.digits);
    return std::make_pair(q, r);
  }

  const typename MidLevel::container_type& get_container() const {
    return digits;
  }

#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_PUBLIC_BUILTIN(type) \
  natural_number_base(type u) : digits() { \
    assign(u); \
  } \
  inline void assign(type u) { \
    MidLevel::set(digits, (type_convert<type>::unsigned_type) u); \
  } \
  static inline type subtract(type x, const natural_number_base& y) { \
    return MidLevel::subtract((type_convert<type>::unsigned_type) x, y.digits); \
  } \
  static inline type remainder(const natural_number_base& x, type y) { \
    return MidLevel::remainder(x.digits, (type_convert<type>::unsigned_type) y); \
  } \
  static inline type divide(type x, const natural_number_base& y) { \
    return MidLevel::divide((type_convert<type>::unsigned_type) x, y.digits); \
  } \
  static inline std::pair<natural_number_base, type> \
        quotrem(const natural_number_base& x, type y) { \
    natural_number_base q; \
    type r = MidLevel::quotrem(q.digits, x.digits, (type_convert<type>::unsigned_type) y); \
    return std::make_pair(q, r); \
  } \
  static inline std::pair<type, natural_number_base> \
        quotrem(type x, const natural_number_base& y) { \
    natural_number_base r; \
    type q = MidLevel::quotrem(r.digits, (type_convert<type>::unsigned_type) x, y.digits); \
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
