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

  static inline void sub(natural_number& r,
      const natural_number& u, const natural_number& v) {
    MidLevel::subtract(r.digits, u.digits, v.digits);
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

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(type, convtype) \
  static inline void add(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::add(r.digits, u.digits, (convtype) v); \
  } \
  static inline void add(natural_number& r, \
      type u, const natural_number& v) { \
    MidLevel::add(r.digits, v.digits, (convtype) u); \
  } \
  static inline void subtract(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::subtract(r.digits, u.digits, (convtype) v); \
  } \
  static inline void subtract(natural_number& r, \
      type u, const natural_number& v) { \
    assign(MidLevel::subtract((convtype) u, v.digits)); \
  } \
  static inline void multiply(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::multiply(r.digits, u.digits, (convtype) v); \
  } \
  static inline void multiply(natural_number& r, \
      type u, const natural_number& v) { \
    MidLevel::multiply(r.digits, v.digits, (convtype) u); \
  } \
  static inline void divide(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::divide(r.digits, u.digits, (convtype) v); \
  } \
  static inline void divide(natural_number& r, \
      type u, const natural_number& v) { \
    assign(divide((convtype) u, v.digits)); \
  } \
  static inline void remainder(natural_number& r, \
      const natural_number& u, type v) { \
    assign(remainder(u.digits, (convtype) v)); \
  } \
  static inline void remainder(natural_number& r, \
      type u, const natural_number& v) { \
    MidLevel::remainder(r.digits, (convtype) u, v.digits); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(int, unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(uint8_t, uint8_t)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(uint16_t, uint16_t)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(uint32_t, uint32_t)
#ifdef SPUTSOFT_HAS_64_BIT_TYPES
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(uint64_t, uint64_t)
#endif // SPUTSOFT_HAS_64_BIT_TYPES

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

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(type, convtype) \
  inline void assign(type u) { \
    MidLevel::set(digits, (convtype) u); \
  } \
  static inline type subtract(type x, const natural_number& y) { \
    return MidLevel::subtract((convtype) x, y.digits); \
  } \
  static inline type remainder(const natural_number& x, type y) { \
    return MidLevel::remainder(x.digits, (convtype) y); \
  } \
  static inline type divide(type x, const natural_number& y) { \
    return MidLevel::divide((convtype) x, y.digits); \
  } \
  static inline std::pair<natural_number, type> \
        quotrem(const natural_number& x, type y) { \
    natural_number q; \
    type r = MidLevel::quotrem(q.digits, x.digits, (convtype) y); \
    return std::make_pair(q, r); \
  } \
  static inline std::pair<type, natural_number> \
        quotrem(type x, const natural_number& y) { \
    natural_number r; \
    type q = MidLevel::quotrem(r.digits, (convtype) x, y.digits); \
    return std::make_pair(q, r); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(int, unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(uint8_t, uint8_t)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(uint16_t, uint16_t)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(uint32_t, uint32_t)
#ifdef SPUTSOFT_HAS_64_BIT_TYPES
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(uint64_t, uint64_t)
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
  //expr& operator=(int v) { n.assign((unsigned) v); return *this; }
  template <typename T>
    expr& operator=(const T& e) { n.assign(e); return *this; }
  template <typename E>
    inline expr& operator+=(const E& e) { n.add_this(e); return *this; }
  template <typename E>
    inline expr& operator-=(const E& e) { n.subtract_this(e); return *this; }
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

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(type) \
template <typename Op, typename ML> \
struct resolve_binary<Op, type, natural_number<ML> > { \
  typedef natural_number<ML> return_type; \
}; \
template <typename Op, typename ML> \
struct resolve_binary<Op, natural_number<ML>, type> { \
  typedef natural_number<ML> return_type; \
}; \
template <typename ML> \
struct resolve_binary<ops::binary::subtract, type, natural_number<ML> > { \
  typedef type return_type; \
}; \
template <typename ML> \
struct resolve_binary<ops::binary::remainder, natural_number<ML>, type> { \
  typedef type return_type; \
}; \
template <typename ML> \
struct resolve_binary<ops::binary::divide, type, natural_number<ML> > { \
  typedef type return_type; \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(int)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(uint8_t)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(uint16_t)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(uint32_t)
#ifdef SPUTSOFT_HAS_64_BIT_TYPES
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(uint64_t)
#endif // SPUTSOFT_HAS_64_BIT_TYPES

/* Specializations for built-in return types */

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(type) \
template <typename ML, typename E1, typename E2> \
class expr<type, wrap::binary<ops::binary::subtract, expr<type, E1>, \
                                  expr<natural_number<ML>, E2> > > { \
private: \
  typedef wrap::binary<ops::binary::subtract, expr<type, E1>, \
                       expr<natural_number<ML>, E2> > E; \
  E e; \
public: \
  expr(const E& _e) : e(_e) {} \
  const E& get_expr() const { return e; } \
  type eval() const { \
    return natural_number<ML>::subtract(e.x.eval(), e.y.eval()); \
  } \
  operator type() const { return eval(); } \
}; \
template <typename ML, typename E1, typename E2> \
class expr<type, wrap::binary<ops::binary::remainder, \
           expr<natural_number<ML>, E1>, expr<type, E2> > > { \
private: \
  typedef wrap::binary<ops::binary::remainder, expr<natural_number<ML>, E1>, \
                       expr<type, E2> > E; \
  E e; \
public: \
  expr(const E& _e) : e(_e) {} \
  const E& get_expr() const { return e; } \
  type eval() const { \
    return natural_number<ML>::remainder(e.x.eval(), e.y.eval()); \
  } \
  operator type() const { return eval(); } \
}; \
template <typename ML, typename E1, typename E2> \
class expr<type, wrap::binary<ops::binary::divide, expr<type, E1>, \
                                  expr<natural_number<ML>, E2> > > { \
private: \
  typedef wrap::binary<ops::binary::divide, expr<type, E1>, \
                       expr<natural_number<ML>, E2> > E; \
  E e; \
public: \
  expr(const E& _e) : e(_e) {} \
  const E& get_expr() const { return e; } \
  type eval() const { \
    return natural_number<ML>::divide(e.x.eval(), e.y.eval()); \
  } \
  operator type() const { return eval(); } \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(int)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(uint8_t)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(uint16_t)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(uint32_t)
#ifdef SPUTSOFT_HAS_64_BIT_TYPES
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(uint64_t)
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

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(type) \
template <typename ML> \
struct quotrem_evaluator<detail::natural_number<ML>, type> { \
  static inline std::pair<detail::natural_number<ML>, type> \
        quotrem(const detail::natural_number<ML>& u, type v) { \
    return detail::natural_number<ML>::quotrem(u, v); \
  } \
}; \
template <typename ML> \
struct quotrem_evaluator<type, detail::natural_number<ML> > { \
  static inline std::pair<type, detail::natural_number<ML> > \
        quotrem(type u, const detail::natural_number<ML>& v) { \
    return detail::natural_number<ML>::quotrem(u, v); \
  } \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(uint8_t)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(uint16_t)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(uint32_t)
#ifdef SPUTSOFT_HAS_64_BIT_TYPES
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(uint64_t)
#endif // SPUTSOFT_HAS_64_BIT_TYPES

} // namespace number_theory
} // namespace math
} // namespace sputsoft

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_

