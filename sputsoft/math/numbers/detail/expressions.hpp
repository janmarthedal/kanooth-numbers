/*
 * File:   math/numbers/detail/expressions.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_DETAIL_EXPRESSIONS_HPP_
#define _SPUTSOFT_MATH_NUMBERS_DETAIL_EXPRESSIONS_HPP_

#include <iostream>

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace detail {

namespace ops {
  namespace binary {
    class add {};
    class subtract {};
    class multiply {};
    class divide {};
    class remainder {};
  }
  namespace unary {
    class identity {};
    class negate {};
    class not_zero {};
  }
}

namespace expr {

  template <typename Op, typename X, typename Y>
  class binary {
  private:
    binary() {}
  public:
    const X& x;
    const Y& y;
    binary(const X& _x, const Y& _y) : x(_x), y(_y) {}
    binary(const binary& e) : x(e.x), y(e.y) {}
  };

  template <typename Op, typename X>
  class unary {
  private:
    const X& x;
    unary() {}
    unary(const unary&) {}
  public:
    unary(const X& _x) : x(_x) {}
    const X& get() const { return x; }
  };  

  template <typename X>
  class unary<ops::unary::identity, X> {
  private:
    X x;
  public:
    unary() : x() {}
    template <typename T> unary(const T& v) : x(v) {}
    template <typename E> unary& operator=(const E& expr)
      { X::assign(x, expr); return *this; }
    const X& get() const { return x; }
  };

}

template <typename T>
std::ostream& operator<<(std::ostream& os, const expr::unary<ops::unary::identity, T>& n)
{
  return os << n.get();
}


/*template <typename Xop, typename X1, typename X2, typename Yop, typename Y1, typename Y2>
expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> >
operator+(const expr::binary<Xop, X1, X2>& x, const expr::binary<Yop, Y1, Y2>& y) \
{ return expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> >(x, y); }

template <typename X, typename Yop, typename Y1, typename Y2>
expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >
operator+(const expr::unary<ops::unary::identity, X>& x, const expr::binary<Yop, Y1, Y2>& y)
{ return expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >(x.get(), y); }

template <typename Xop, typename X1, typename X2, typename Y>
expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, Y>
operator+(const expr::binary<Xop, X1, X2>& x, const expr::unary<ops::unary::identity, Y>& y)
{ return expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, Y>(x, y.get()); }

template <typename X, typename Yop, typename Y1, typename Y2>
expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >
operator+(const X& x, const expr::binary<Yop, Y1, Y2>& y)
{ return expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >(x, y); }

template <typename Xop, typename X1, typename X2, typename Y>
expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, Y>
operator+(const expr::binary<Xop, X1, X2>& x, const Y& y)
{ return expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, Y>(x, y); }

template <typename X, typename Y>
expr::binary<ops::binary::add, X, Y>
operator+(const expr::unary<ops::unary::identity, X>& x, const expr::unary<ops::unary::identity, Y>& y)
{ return expr::binary<ops::binary::add, X, Y>(x.get(), y.get()); }

template <typename X, typename Y>
expr::binary<ops::binary::add, X, Y>
operator+(const expr::unary<ops::unary::identity, X>& x, const Y& y)
{ return expr::binary<ops::binary::add, X, Y>(x.get(), y); }

template <typename X, typename Y>
expr::binary<ops::binary::add, X, Y>
operator+(const X& x, const expr::unary<ops::unary::identity, Y>& y)
{ return expr::binary<ops::binary::add, X, Y>(x, y.get()); }
*/

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_BIN_BIN(op_overload, op_type) \
template <typename Xop, typename X1, typename X2, typename Yop, typename Y1, typename Y2> \
expr::binary<op_type, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> > \
op_overload(const expr::binary<Xop, X1, X2>& x, const expr::binary<Yop, Y1, Y2>& y) \
{ return expr::binary<op_type, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> >(x, y); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_ID_BIN(op_overload, op_type) \
template <typename X, typename Yop, typename Y1, typename Y2> \
expr::binary<op_type, X, expr::binary<Yop, Y1, Y2> > \
op_overload(const expr::unary<ops::unary::identity, X>& x, const expr::binary<Yop, Y1, Y2>& y) \
{ return expr::binary<op_type, X, expr::binary<Yop, Y1, Y2> >(x.get(), y); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_BIN_ID(op_overload, op_type) \
template <typename Xop, typename X1, typename X2, typename Y> \
expr::binary<op_type, expr::binary<Xop, X1, X2>, Y> \
op_overload(const expr::binary<Xop, X1, X2>& x, const expr::unary<ops::unary::identity, Y>& y) \
{ return expr::binary<op_type, expr::binary<Xop, X1, X2>, Y>(x, y.get()); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_X_BIN(op_overload, op_type) \
template <typename X, typename Yop, typename Y1, typename Y2> \
expr::binary<op_type, X, expr::binary<Yop, Y1, Y2> > \
op_overload(const X& x, const expr::binary<Yop, Y1, Y2>& y) \
{ return expr::binary<op_type, X, expr::binary<Yop, Y1, Y2> >(x, y); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_BIN_X(op_overload, op_type) \
template <typename Xop, typename X1, typename X2, typename Y> \
expr::binary<op_type, expr::binary<Xop, X1, X2>, Y> \
op_overload(const expr::binary<Xop, X1, X2>& x, const Y& y) \
{ return expr::binary<op_type, expr::binary<Xop, X1, X2>, Y>(x, y); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_ID_ID(op_overload, op_type) \
template <typename X, typename Y> \
expr::binary<op_type, X, Y> \
op_overload(const expr::unary<ops::unary::identity, X>& x, const expr::unary<ops::unary::identity, Y>& y) \
{ return expr::binary<op_type, X, Y>(x.get(), y.get()); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_ID_X(op_overload, op_type) \
template <typename X, typename Y> \
expr::binary<op_type, X, Y> \
op_overload(const expr::unary<ops::unary::identity, X>& x, const Y& y) \
{ return expr::binary<op_type, X, Y>(x.get(), y); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE_X_ID(op_overload, op_type) \
template <typename X, typename Y> \
expr::binary<op_type, X, Y> \
op_overload(const X& x, const expr::unary<ops::unary::identity, Y>& y) \
{ return expr::binary<op_type, X, Y>(x, y.get()); }

#define SPUTSOFT_MATH_NUMBER_BINARY_PARSE(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_BIN_BIN(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_ID_BIN(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_BIN_ID(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_X_BIN(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_BIN_X(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_ID_ID(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_ID_X(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_BINARY_PARSE_X_ID(op_overload, op_type)

SPUTSOFT_MATH_NUMBER_BINARY_PARSE(operator+, ops::binary::add)
SPUTSOFT_MATH_NUMBER_BINARY_PARSE(operator-, ops::binary::subtract)
SPUTSOFT_MATH_NUMBER_BINARY_PARSE(operator*, ops::binary::multiply)
SPUTSOFT_MATH_NUMBER_BINARY_PARSE(operator/, ops::binary::divide)
SPUTSOFT_MATH_NUMBER_BINARY_PARSE(operator%, ops::binary::remainder)

#define SPUTSOFT_MATH_NUMBER_UNARY_PARSE_BIN(op_overload, op_type) \
template <typename Xop, typename X1, typename X2> \
expr::unary<op_type, expr::binary<Xop, X1, X2> > \
op_overload(const expr::binary<Xop, X1, X2>& x) \
{ return expr::unary<op_type, expr::binary<Xop, X1, X2> >(x); }

#define SPUTSOFT_MATH_NUMBER_UNARY_PARSE_UN(op_overload, op_type) \
template <typename Xop, typename X> \
expr::unary<op_type, expr::unary<Xop, X> > \
op_overload(const expr::unary<Xop, X>& x) \
{ return expr::unary<op_type, expr::unary<Xop, X> >(x); }

#define SPUTSOFT_MATH_NUMBER_UNARY_PARSE(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_UNARY_PARSE_BIN(op_overload, op_type) \
  SPUTSOFT_MATH_NUMBER_UNARY_PARSE_UN(op_overload, op_type)

SPUTSOFT_MATH_NUMBER_UNARY_PARSE(operator!, ops::unary::not_zero)

// unary +

template <typename Xop, typename X1, typename X2>
expr::binary<Xop, X1, X2>
operator+(const expr::binary<Xop, X1, X2>& e)
{ return e; }

template <typename Xop, typename X>
expr::unary<Xop, X>
operator+(const expr::unary<Xop, X>& x)
{ return x; }

// unary -

SPUTSOFT_MATH_NUMBER_UNARY_PARSE(operator-, ops::unary::negate)

template <typename X>
expr::unary<ops::unary::identity, X>
operator-(const expr::unary<ops::unary::negate, X>& e)
{ return expr::unary<ops::unary::identity, X>(e.get()); }

template <typename X>
expr::unary<ops::unary::negate, X>
operator-(const expr::unary<ops::unary::identity, X>& e)
{ return expr::unary<ops::unary::negate, X>(e.get()); }


} // namespace detail
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_EXPRESSIONS_HPP_

