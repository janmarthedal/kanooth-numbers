/*
 * File:   numbers/detail/natural_number.hpp
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

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_

#include <iostream>
#include <sputsoft/types.hpp>
#include <sputsoft/number_theory/common.hpp>
#include <sputsoft/numbers/detail/expressions.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename MidLevel>
class natural_number {
/*public:
  typedef typename MidLevel::container_type container_type;*/
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

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(type) \
  static inline void add(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::add(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void add(natural_number& r, \
      type u, const natural_number& v) { \
    MidLevel::add(r.digits, v.digits, (type_convert<type>::unsigned_type) u); \
  } \
  static inline void subtract(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::subtract(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void subtract(natural_number& r, \
      type u, const natural_number& v) { \
    assign(MidLevel::subtract((type_convert<type>::unsigned_type) u, v.digits)); \
  } \
  static inline void multiply(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::multiply(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void multiply(natural_number& r, \
      type u, const natural_number& v) { \
    MidLevel::multiply(r.digits, v.digits, (type_convert<type>::unsigned_type) u); \
  } \
  static inline void divide(natural_number& r, \
      const natural_number& u, type v) { \
    MidLevel::divide(r.digits, u.digits, (type_convert<type>::unsigned_type) v); \
  } \
  static inline void divide(natural_number& r, \
      type u, const natural_number& v) { \
    assign(divide((type_convert<type>::unsigned_type) u, v.digits)); \
  } \
  static inline void remainder(natural_number& r, \
      const natural_number& u, type v) { \
    assign(remainder(u.digits, (type_convert<type>::unsigned_type) v)); \
  } \
  static inline void remainder(natural_number& r, \
      type u, const natural_number& v) { \
    MidLevel::remainder(r.digits, (type_convert<type>::unsigned_type) u, v.digits); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(unsigned long)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(signed short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(signed)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PRIVATE_BUILTIN(signed long)

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

  const typename MidLevel::container_type& get_container() const {
    return digits;
  }

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(type) \
  inline void assign(type u) { \
    MidLevel::set(digits, (type_convert<type>::unsigned_type) u); \
  } \
  static inline type subtract(type x, const natural_number& y) { \
    return MidLevel::subtract((type_convert<type>::unsigned_type) x, y.digits); \
  } \
  static inline type remainder(const natural_number& x, type y) { \
    return MidLevel::remainder(x.digits, (type_convert<type>::unsigned_type) y); \
  } \
  static inline type divide(type x, const natural_number& y) { \
    return MidLevel::divide((type_convert<type>::unsigned_type) x, y.digits); \
  } \
  static inline std::pair<natural_number, type> \
        quotrem(const natural_number& x, type y) { \
    natural_number q; \
    type r = MidLevel::quotrem(q.digits, x.digits, (type_convert<type>::unsigned_type) y); \
    return std::make_pair(q, r); \
  } \
  static inline std::pair<type, natural_number> \
        quotrem(type x, const natural_number& y) { \
    natural_number r; \
    type q = MidLevel::quotrem(r.digits, (type_convert<type>::unsigned_type) x, y.digits); \
    return std::make_pair(q, r); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(unsigned long)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(signed short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(signed)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_PUBLIC_BUILTIN(signed long)

};


template <typename MidLevel>
class expr<natural_number<MidLevel>, natural_number<MidLevel> > {
private:
  natural_number<MidLevel> n;
  explicit expr(const natural_number<MidLevel>& _n) : n(_n) {}
public:
  typedef typename MidLevel::container_type container_type;
  expr() : n() {}
  template <typename T>
    expr(const T& e) : n() { n.assign(e); }
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
  const container_type& get_container() const {
    return n.get_container();
  }
  static std::pair<expr, expr> quotrem(const expr& u, const expr& v) {
    std::pair<natural_number<MidLevel>, natural_number<MidLevel> > qr =
            natural_number<MidLevel>::quotrem(u.n, v.n);
    return std::make_pair(expr(qr.first), expr(qr.second));
  }
#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_EXPR_QUOTREM(type) \
  static std::pair<expr, type> quotrem(const expr& u, type v) { \
    std::pair<natural_number<MidLevel>, type> qr = \
            natural_number<MidLevel>::quotrem(u.n, v); \
    return std::make_pair(expr(qr.first), qr.second); \
  } \
  static std::pair<type, expr> quotrem(type u, const expr& v) { \
    std::pair<type, natural_number<MidLevel> > qr = \
            natural_number<MidLevel>::quotrem(u, v.n); \
    return std::make_pair(qr.first, expr(qr.second)); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_EXPR_QUOTREM(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_EXPR_QUOTREM(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_EXPR_QUOTREM(unsigned long)

};

template <typename MidLevel>
std::ostream& operator<<(std::ostream& os,
        const expr<natural_number<MidLevel> >& n)
{
  typedef typename expr<natural_number<MidLevel> >::container_type container_type;
  const container_type& con = n.get_container();
  if (con.is_empty())
    os << "0";
  else
    os << con[0];
  return os;
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

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(unsigned short)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(unsigned)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(unsigned long)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(signed short)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(signed)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_RESOLVE(signed long)

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

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(unsigned short)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(unsigned)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(unsigned long)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(signed short)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(signed)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_EXPR(signed long)

} // namespace detail
} // namespace numbers
} // namespace sputsoft

namespace sputsoft {
namespace number_theory {

/* Specializations */

template <typename ML>
struct quotrem_evaluator<numbers::detail::expr<numbers::detail::natural_number<ML> >,
                         numbers::detail::expr<numbers::detail::natural_number<ML> > > {
  static inline std::pair<numbers::detail::expr<numbers::detail::natural_number<ML> >,
                          numbers::detail::expr<numbers::detail::natural_number<ML> > >
        quotrem(const numbers::detail::expr<numbers::detail::natural_number<ML> >& u,
                const numbers::detail::expr<numbers::detail::natural_number<ML> >& v)
  {
    return numbers::detail::expr<numbers::detail::natural_number<ML> >::quotrem(u, v);
  }
};

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(type) \
template <typename ML> \
struct quotrem_evaluator<numbers::detail::expr<numbers::detail::natural_number<ML> >, type> { \
  static inline std::pair<numbers::detail::expr<numbers::detail::natural_number<ML> >, type> \
        quotrem(const numbers::detail::expr<numbers::detail::natural_number<ML> >& u, type v) { \
    return numbers::detail::expr<numbers::detail::natural_number<ML> >::quotrem(u, v); \
  } \
}; \
template <typename ML> \
struct quotrem_evaluator<type, numbers::detail::expr<numbers::detail::natural_number<ML> > > { \
  static inline std::pair<type, numbers::detail::expr<numbers::detail::natural_number<ML> > > \
        quotrem(type u, const numbers::detail::expr<numbers::detail::natural_number<ML> >& v) { \
    return numbers::detail::expr<numbers::detail::natural_number<ML> >::quotrem(u, v); \
  } \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(unsigned short)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(unsigned)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(unsigned long)

} // namespace number_theory
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_
