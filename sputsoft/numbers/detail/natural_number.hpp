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
#include <sputsoft/numbers/detail/natural_number_base.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename MidLevel>
class expr<natural_number_base<MidLevel>, natural_number_base<MidLevel> > {
private:
  natural_number_base<MidLevel> n;
  explicit expr(const natural_number_base<MidLevel>& _n) : n(_n) {}
public:
  typedef typename MidLevel::container_type container_type;

  expr() : n() {}
  expr(unsigned short u) : n(u) {}
  expr(unsigned u)       : n(u) {}
  expr(unsigned long u)  : n(u) {}
  expr(signed short u)   : n(u) {}
  expr(signed u)         : n(u) {}
  expr(signed long u)    : n(u) {}
  expr(const expr& e) : n(e.get_expr()) {}
  template <typename E>
  expr(const expr<natural_number_base<MidLevel>, E>& e) : n(e.get_expr()) {}
  
  expr& operator=(unsigned short u) { n.assign(u); return *this; }
  expr& operator=(unsigned u)       { n.assign(u); return *this; }
  expr& operator=(unsigned long u)  { n.assign(u); return *this; }
  expr& operator=(signed short u)   { n.assign(u); return *this; }
  expr& operator=(signed u)         { n.assign(u); return *this; }
  expr& operator=(signed long u)    { n.assign(u); return *this; }

  expr& operator=(const expr& e) {
    n.assign_expr(e.get_expr());
    return *this;
  }
  template <typename E>
  expr& operator=(const expr<natural_number_base<MidLevel>, E>& e) {
    n.assign_expr(e.get_expr());
    return *this;
  }

  template <typename E>
  inline expr& operator+=(const E& e) { n.add_this(e); return *this; }
  template <typename E>
  inline expr& operator-=(const E& e) { n.subtract_this(e); return *this; }
  template <typename E>
  inline expr& operator/=(const E& e) { n.divide_this(e); return *this; }
  template <typename E>
  inline expr& operator%=(const E& e) { n.remainder_this(e); return *this; }
  inline operator bool() const { return !n.is_zero(); }
  inline const natural_number_base<MidLevel>& get_expr() const { return n; }
  inline const natural_number_base<MidLevel>& eval() const { return n; }
  const container_type& get_container() const {
    return n.get_container();
  }
  static std::pair<expr, expr> quotrem(const expr& u, const expr& v) {
    std::pair<natural_number_base<MidLevel>, natural_number_base<MidLevel> > qr =
            natural_number_base<MidLevel>::quotrem(u.n, v.n);
    return std::make_pair(expr(qr.first), expr(qr.second));
  }
#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_EXPR_QUOTREM(type) \
  static std::pair<expr, type> quotrem(const expr& u, type v) { \
    std::pair<natural_number_base<MidLevel>, type> qr = \
            natural_number_base<MidLevel>::quotrem(u.n, v); \
    return std::make_pair(expr(qr.first), qr.second); \
  } \
  static std::pair<type, expr> quotrem(type u, const expr& v) { \
    std::pair<type, natural_number_base<MidLevel> > qr = \
            natural_number_base<MidLevel>::quotrem(u, v.n); \
    return std::make_pair(qr.first, expr(qr.second)); \
  }

  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_EXPR_QUOTREM(unsigned short)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_EXPR_QUOTREM(unsigned)
  SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_EXPR_QUOTREM(unsigned long)

};

template <typename MidLevel>
std::ostream& operator<<(std::ostream& os,
        const expr<natural_number_base<MidLevel> >& n)
{
  if (n) {
    typedef expr<natural_number_base<MidLevel> > num;
    typedef typename num::container_type container_type;
    typedef typename container_type::digit_type digit_type;
    const container_type& con = n.get_container();
    unsigned max_digits = con.size() * boost::integer_traits<digit_type>::digits / 3 + 1;
    char st[max_digits];
    char* p = st + max_digits;
    *--p = 0;
    num t = n;
    while (t) {
      std::pair<num, short unsigned> qr = number_theory::quotrem(t, (short unsigned) 10);
      t = qr.first;
      *--p = (char) qr.second + '0';
    }
    os << p;
  } else
    os << "0";
  return os;
}


/* Return type resolving */

template <typename Op, typename ML>
struct resolve_binary<Op, natural_number_base<ML>, natural_number_base<ML> > {
  typedef natural_number_base<ML> return_type;
};

#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(type) \
template <typename Op, typename ML> \
struct resolve_binary<Op, type, natural_number_base<ML> > { \
  typedef natural_number_base<ML> return_type; \
}; \
template <typename Op, typename ML> \
struct resolve_binary<Op, natural_number_base<ML>, type> { \
  typedef natural_number_base<ML> return_type; \
}; \
template <typename ML> \
struct resolve_binary<ops::binary::subtract, type, natural_number_base<ML> > { \
  typedef type return_type; \
}; \
template <typename ML> \
struct resolve_binary<ops::binary::remainder, natural_number_base<ML>, type> { \
  typedef type return_type; \
}; \
template <typename ML> \
struct resolve_binary<ops::binary::divide, type, natural_number_base<ML> > { \
  typedef type return_type; \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(unsigned short)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(unsigned)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(unsigned long)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(signed short)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(signed)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_RESOLVE(signed long)

/* Specializations for built-in return types */

#define SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(type) \
template <typename ML, typename E1, typename E2> \
class expr<type, wrap::binary<ops::binary::subtract, expr<type, E1>, \
                                  expr<natural_number_base<ML>, E2> > > { \
private: \
  typedef wrap::binary<ops::binary::subtract, expr<type, E1>, \
                       expr<natural_number_base<ML>, E2> > E; \
  E e; \
public: \
  expr(const E& _e) : e(_e) {} \
  const E& get_expr() const { return e; } \
  type eval() const { \
    return natural_number_base<ML>::subtract(e.x.eval(), e.y.eval()); \
  } \
  operator type() const { return eval(); } \
}; \
template <typename ML, typename E1, typename E2> \
class expr<type, wrap::binary<ops::binary::remainder, \
           expr<natural_number_base<ML>, E1>, expr<type, E2> > > { \
private: \
  typedef wrap::binary<ops::binary::remainder, expr<natural_number_base<ML>, E1>, \
                       expr<type, E2> > E; \
  E e; \
public: \
  expr(const E& _e) : e(_e) {} \
  const E& get_expr() const { return e; } \
  type eval() const { \
    return natural_number_base<ML>::remainder(e.x.eval(), e.y.eval()); \
  } \
  operator type() const { return eval(); } \
}; \
template <typename ML, typename E1, typename E2> \
class expr<type, wrap::binary<ops::binary::divide, expr<type, E1>, \
                                  expr<natural_number_base<ML>, E2> > > { \
private: \
  typedef wrap::binary<ops::binary::divide, expr<type, E1>, \
                       expr<natural_number_base<ML>, E2> > E; \
  E e; \
public: \
  expr(const E& _e) : e(_e) {} \
  const E& get_expr() const { return e; } \
  type eval() const { \
    return natural_number_base<ML>::divide(e.x.eval(), e.y.eval()); \
  } \
  operator type() const { return eval(); } \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(unsigned short)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(unsigned)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(unsigned long)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(signed short)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(signed)
SPUTSOFT_MATH_NUMBERS_DETAIL_natural_number_base_SPEC_EXPR(signed long)

} // namespace detail
} // namespace numbers
} // namespace sputsoft

namespace sputsoft {
namespace number_theory {

/* Specializations */

template <typename ML>
struct quotrem_evaluator<numbers::detail::expr<numbers::detail::natural_number_base<ML> >,
                         numbers::detail::expr<numbers::detail::natural_number_base<ML> > > {
  static inline std::pair<numbers::detail::expr<numbers::detail::natural_number_base<ML> >,
                          numbers::detail::expr<numbers::detail::natural_number_base<ML> > >
        quotrem(const numbers::detail::expr<numbers::detail::natural_number_base<ML> >& u,
                const numbers::detail::expr<numbers::detail::natural_number_base<ML> >& v)
  {
    return numbers::detail::expr<numbers::detail::natural_number_base<ML> >::quotrem(u, v);
  }
};

#define SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(type) \
template <typename ML> \
struct quotrem_evaluator<numbers::detail::expr<numbers::detail::natural_number_base<ML> >, type> { \
  static inline std::pair<numbers::detail::expr<numbers::detail::natural_number_base<ML> >, type> \
        quotrem(const numbers::detail::expr<numbers::detail::natural_number_base<ML> >& u, type v) { \
    return numbers::detail::expr<numbers::detail::natural_number_base<ML> >::quotrem(u, v); \
  } \
}; \
template <typename ML> \
struct quotrem_evaluator<type, numbers::detail::expr<numbers::detail::natural_number_base<ML> > > { \
  static inline std::pair<type, numbers::detail::expr<numbers::detail::natural_number_base<ML> > > \
        quotrem(type u, const numbers::detail::expr<numbers::detail::natural_number_base<ML> >& v) { \
    return numbers::detail::expr<numbers::detail::natural_number_base<ML> >::quotrem(u, v); \
  } \
};

SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(unsigned short)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(unsigned)
SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_SPEC_NUMTH(unsigned long)

} // namespace number_theory
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_
