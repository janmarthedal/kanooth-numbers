/*
 * File:   math/numbers/detail/natural_number.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_

#include <iostream>
#include <sputsoft/types.hpp>
#include <sputsoft/math/number_theory/common.hpp>
#include <sputsoft/math/numbers/detail/expressions.hpp>

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace detail {

template <typename MidLevel>
class natural_number {
private:
  typename MidLevel::container_type digits;

  static inline void add(natural_number& r,
      const natural_number& u, const natural_number& v) {
    MidLevel::add(r.digits, u.digits, v.digits);
  }

  static inline void multiply(natural_number& r,
      const natural_number& u, const natural_number& v) {
    MidLevel::multiply(r.digits, u.digits, v.digits);
  }

  static inline void divide(natural_number& r,
      const natural_number& u, const natural_number& v) {
    MidLevel::divide(r.digits, u.digits, v.digits);
  }

  static inline void remainder(natural_number& r,
      const natural_number& u, const natural_number& v) {
    MidLevel::remainder(r.digits, u.digits, v.digits);
  }

#ifdef SPUTSOFT_HAS_64_BIT_TYPES

  static inline void add(natural_number& r,
      const natural_number& u, uint64_t v) {
    MidLevel::add(r.digits, u.digits, v);
  }

  static inline void add(natural_number& r,
      uint64_t u, const natural_number& v) {
    MidLevel::add(r.digits, v.digits, u);
  }

  static inline void multiply(natural_number& r,
      const natural_number& u, uint64_t v) {
    MidLevel::multiply(r.digits, u.digits, v);
  }

  static inline void multiply(natural_number& r,
      uint64_t u, const natural_number& v) {
    MidLevel::multiply(r.digits, v.digits, u);
  }

  static inline void divide(natural_number& r,
      const natural_number& u, uint64_t v) {
    MidLevel::divide(r.digits, u.digits, v);
  }

  static inline void divide(natural_number& r,
      uint64_t u, const natural_number& v) {
    assign(divide(u, v.digits));
  }

  static inline void remainder(natural_number& r,
      const natural_number& u, uint64_t v) {
    assign(remainder(u.digits, v));
  }

  static inline void remainder(natural_number& r,
      uint64_t u, const natural_number& v) {
    MidLevel::remainder(r.digits, u, v.digits);
  }

#endif // SPUTSOFT_HAS_64_BIT_TYPES

  template <typename R1, typename E1, typename R2, typename E2>
  inline void assign_expr(const wrap::binary<ops::binary::add,
      expr<R1, E1>, expr<R2, E2> >& e) {
    add(*this, e.x.eval(), e.y.eval());
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

public:
  natural_number() : digits() {}

  template <typename R, typename E>
  inline void add_this(const expr<R, E>& e) {
    add(*this, *this, e.eval());
  }

  template <typename T>
  inline void add_this(const T& v) {
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

  template <typename E>
  inline void assign(const expr<natural_number, E>& e) {
    assign_expr(e.get_expr());
  }

  inline std::string to_string(unsigned base) const {
    return MidLevel::to_string(digits, base);
  }

  inline bool is_zero() const {
    return digits.is_empty();
  }

  static inline std::pair<natural_number, natural_number>
        quotrem(const natural_number& x, const natural_number& y) {
    natural_number q, r;
    MidLevel::quotrem(q.digits, r.digits, x.digits, y.digits);
    return std::make_pair(q, r);
  }

#ifdef SPUTSOFT_HAS_64_BIT_TYPES

  inline void assign(uint64_t u) {
    MidLevel::set(digits, u);
  }

  static inline uint64_t remainder(const natural_number& x, uint64_t y) {
    return MidLevel::remainder(x.digits, y);
  }

  static inline uint64_t divide(uint64_t x, const natural_number& y) {
    return MidLevel::divide(x, y.digits);
  }

  static inline std::pair<natural_number, uint64_t>
        quotrem(const natural_number& x, uint64_t y) {
    natural_number q;
    uint64_t r = MidLevel::quotrem(q.digits, x.digits, y);
    return std::make_pair(q, r);
  }

  static inline std::pair<uint64_t, natural_number>
        quotrem(uint64_t x, const natural_number& y) {
    natural_number r;
    uint64_t q = MidLevel::quotrem(r.digits, x, y.digits);
    return std::make_pair(q, r);
  }

#endif // SPUTSOFT_HAS_64_BIT_TYPES

};


template <typename MidLevel>
class expr<natural_number<MidLevel>, natural_number<MidLevel> > {
private:
  natural_number<MidLevel> n;
public:
  expr() : n() {}
  template <typename T>
    expr(const T& e) : n() { n.assign(e); }
  template <typename T>
    expr& operator=(const T& e) { n.assign(e); return *this; }
  template <typename E>
    inline expr& operator+=(const E& e) { n.add_this(e); return *this; }
  template <typename E>
    inline expr& operator/=(const E& e) { n.divide_this(e); return *this; }
  template <typename E>
    inline expr& operator%=(const E& e) { n.remainder_this(e); return *this; }
  inline operator bool() const { return !n.is_zero(); }
  inline const natural_number<MidLevel>& get_expr() const { return n; }
  inline const natural_number<MidLevel>& eval() const { return n; }
};

template <typename MidLevel>
std::ostream& operator<<(std::ostream& os,
        const expr<natural_number<MidLevel> >& n)
{
  return os << n.get_expr().to_string(10);
}


/* Return type resolving */

template <typename Op, typename ML>
struct resolve_binary<Op, natural_number<ML>, natural_number<ML> > {
  typedef natural_number<ML> return_type;
};

#ifdef SPUTSOFT_HAS_64_BIT_TYPES

template <typename Op, typename ML>
struct resolve_binary<Op, uint64_t, natural_number<ML> > {
  typedef natural_number<ML> return_type;
};

template <typename Op, typename ML>
struct resolve_binary<Op, natural_number<ML>, uint64_t> {
  typedef natural_number<ML> return_type;
};

template <typename ML>
struct resolve_binary<ops::binary::remainder, natural_number<ML>, uint64_t> {
  typedef uint64_t return_type;
};

template <typename ML>
struct resolve_binary<ops::binary::divide, uint64_t, natural_number<ML> > {
  typedef uint64_t return_type;
};

#endif // SPUTSOFT_HAS_64_BIT_TYPES

/* Specializations for built-in return types */

#ifdef SPUTSOFT_HAS_64_BIT_TYPES

template <typename ML, typename E1, typename E2>
class expr<uint64_t, wrap::binary<ops::binary::remainder, expr<natural_number<ML>, E1>, expr<uint64_t, E2> > > {
private:
  typedef wrap::binary<ops::binary::remainder, expr<natural_number<ML>, E1>, expr<uint64_t, E2> > E;
  E e;
public:
  expr(const E& _e) : e(_e) {}
  const E& get_expr() const { return e; }
  uint64_t eval() const {
    return natural_number<ML>::remainder(e.x.eval(), e.y.eval());
  }
  operator uint64_t() const { return eval(); }
};

template <typename ML, typename E1, typename E2>
class expr<uint64_t, wrap::binary<ops::binary::divide, expr<uint64_t, E1>,
                                  expr<natural_number<ML>, E2> > > {
private:
  typedef wrap::binary<ops::binary::divide, expr<uint64_t, E1>,
                       expr<natural_number<ML>, E2> > E;
  E e;
public:
  expr(const E& _e) : e(_e) {}
  const E& get_expr() const { return e; }
  uint64_t eval() const {
    return natural_number<ML>::divide(e.x.eval(), e.y.eval());
  }
  operator uint64_t() const { return eval(); }
};

#endif // SPUTSOFT_HAS_64_BIT_TYPES

} // namespace detail
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

namespace sputsoft {
namespace math {
namespace number_theory {

/* Specializations */

template <typename ML>
struct quotrem_evaluator<detail::natural_number<ML>, detail::natural_number<ML> > {
  static inline std::pair<detail::natural_number<ML>, detail::natural_number<ML> >
        quotrem(const detail::natural_number<ML>& u, const detail::natural_number<ML>& v) {
    return detail::natural_number<ML>::quotrem(u, v);
  }
};

#ifdef SPUTSOFT_HAS_64_BIT_TYPES

template <typename ML>
struct quotrem_evaluator<detail::natural_number<ML>, uint64_t> {
  static inline std::pair<detail::natural_number<ML>, uint64_t>
        quotrem(const detail::natural_number<ML>& u, uint64_t v) {
    return detail::natural_number<ML>::quotrem(u, v);
  }
};

template <typename ML>
struct quotrem_evaluator<uint64_t, detail::natural_number<ML> > {
  static inline std::pair<uint64_t, detail::natural_number<ML> >
        quotrem(uint64_t u, const detail::natural_number<ML>& v) {
    return detail::natural_number<ML>::quotrem(u, v);
  }
};

#endif // SPUTSOFT_HAS_64_BIT_TYPES

} // namespace number_theory
} // namespace math
} // namespace sputsoft

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_

