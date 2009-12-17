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
//#include <boost/cstdint.hpp>
#include <sputsoft/math/numbers/detail/expressions.hpp>

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace detail {

template <typename MidLevel>
class natural_number {
private:
  typename MidLevel::container_type digits;

  template <typename E1, typename E2>
  static void assign_expr(natural_number& r, const wrap::binary<ops::binary::add,
      expr<natural_number, E1>, expr<natural_number, E2> >& e) {
    MidLevel::add(r.digits, e.x.eval().digits, e.y.eval().digits);
  }

  template <typename E1, typename E2>
  static void assign_expr(natural_number& r, const wrap::binary<ops::binary::add,
      expr<natural_number, E1>, expr<unsigned, E2> >& e) {
    MidLevel::add(r.digits, e.x.eval().digits, e.y.eval());
  }

  template <typename E1, typename E2>
  static void assign_expr(natural_number& r, const wrap::binary<ops::binary::add,
      expr<unsigned, E1>, expr<natural_number, E2> >& e) {
    MidLevel::add(r.digits, e.y.eval().digits, e.x.eval());
  }

  static void assign(natural_number& r, unsigned u) {
    MidLevel::set(r.digits, u);
  }

public:
  natural_number() : digits() {}
  template <typename E>
  natural_number(const expr<natural_number, E>& e) : digits() {
    assign_expr(*this, e.get_expr());
  }
  template <typename T>
  natural_number(const T& e) : digits() {
    assign(*this, e);
  }
  template <typename E>
  natural_number& operator=(const expr<natural_number, E>& e) {
    assign_expr(*this, e.get_expr());
    return *this;
  }
  template <typename T>
  natural_number& operator=(const T& e) {
    assign(*this, e);
    return *this;
  }
  template <typename E>
  natural_number& operator+=(const expr<natural_number, E>& e) {
    MidLevel::add(digits, digits, e.eval().digits);
    return *this;
  }
  template <typename E>
  natural_number& operator+=(const expr<unsigned, E>& e) {
    return operator+=(e.eval());
  }  
  natural_number& operator+=(unsigned v) {
    MidLevel::add(digits, digits, v);
    return *this;
  }  

  inline std::string to_string(unsigned base) const {
    return MidLevel::to_string(digits, base);
  }

  static unsigned remainder(const natural_number& x, unsigned y) {
    return MidLevel::remainder(x.digits, y);
  }

};

/* Return type resolving */

template <typename Op, typename ML>
struct resolve_binary<Op, natural_number<ML>, natural_number<ML> > {
  typedef natural_number<ML> return_type;
};

template <typename Op, typename ML>
struct resolve_binary<Op, unsigned, natural_number<ML> > {
  typedef natural_number<ML> return_type;
};

template <typename Op, typename ML>
struct resolve_binary<Op, natural_number<ML>, unsigned> {
  typedef natural_number<ML> return_type;
};

template <typename ML>
struct resolve_binary<ops::binary::remainder, natural_number<ML>, unsigned> {
  typedef unsigned return_type;
};

/* Specializations for built-in return types */

template <typename ML, typename E1, typename E2>
class expr<unsigned, wrap::binary<ops::binary::remainder, expr<natural_number<ML>, E1>, expr<unsigned, E2> > > {
private:
  typedef wrap::binary<ops::binary::remainder, expr<natural_number<ML>, E1>, expr<unsigned, E2> > E;
  E e;
public:
  expr(const E& _e) : e(_e) {}
  const E& get_expr() const { return e; }
  unsigned eval() const {
    return natural_number<ML>::remainder(e.x.eval(), e.y.eval());
  }
  operator unsigned() const { return eval(); }
};


} // namespace detail
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_NATURAL_NUMBER_HPP_

