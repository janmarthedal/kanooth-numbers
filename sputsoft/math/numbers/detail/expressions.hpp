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
    class negate {};
    class nonzero {};
  }
}

namespace wrap {

  template <typename R, typename E> class expr;
  template <class T>
  struct resolve_ref {
    typedef T ref_type;
  };
  template <class R, class E>
  struct resolve_ref<expr<R, E> > {
    typedef const expr<R, E>& ref_type;
  };

  template <typename Op, typename X, typename Y>
  class binary {
  private:
    binary() {}
  public:
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
    binary(const X& _x, const Y& _y) : x(_x), y(_y) {}
  };

  template <typename Op, typename X>
  class unary {
  private:
    unary() {}
  public:
    typename resolve_ref<X>::ref_type x;
    unary(const X& _x) : x(_x) {}
  };  

}

template <typename R, typename E>
class expr {
private:
  E e;
public:
  expr(const E& _e) : e(_e) {}
  const E& get_expr() const { return e; }
  R eval() const { return R(e); }
};

template <typename T>
class expr<T, T> {
private:
  T n;
public:
  expr() : n() {}
  template <typename E>
  expr(const E& e) : n(e) {}
  template <typename E>
  expr& operator=(const E& e) { n = e; return *this; }
  const T& get_expr() const { return n; }
  const T& eval() const { return n; }
  template <typename E>
  expr& operator+=(const E& e) { n += e; return *this; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const expr<T, T>& n)
{
  return os << n.get_expr().to_string(10);
}

template <typename Op, typename X, typename Y> struct resolve_binary;

#define SPUTSOFT_MATH_NUMBERS_DETAIL_BINARY_OPS(op_overload, op_type) \
\
template <typename R1, typename E1, typename R2, typename E2> \
expr<typename resolve_binary<op_type, R1, R2>::return_type, \
  wrap::binary<op_type, expr<R1, E1>, expr<R2, E2> > > \
op_overload(const expr<R1, E1>& x, const expr<R2, E2>& y) { \
  return expr<typename resolve_binary<op_type, R1, R2>::return_type, \
              wrap::binary<op_type, expr<R1, E1>, expr<R2, E2> > > \
    (wrap::binary<op_type, expr<R1, E1>, expr<R2, E2> >(x, y)); } \
\
template <typename R, typename E, typename T> \
expr<typename resolve_binary<op_type, R, T>::return_type, \
  wrap::binary<op_type, expr<R, E>, expr<T, T> > > \
op_overload(const expr<R, E>& x, const T& y) { \
  return expr<typename resolve_binary<op_type, R, T>::return_type, \
              wrap::binary<op_type, expr<R, E>, expr<T, T> > > \
    (wrap::binary<op_type, expr<R, E>, expr<T, T> >(x, expr<T, T>(y))); } \
\
template <typename R, typename E, typename T> \
expr<typename resolve_binary<op_type, T, R>::return_type, \
  wrap::binary<op_type, expr<T, T>, expr<R, E> > > \
op_overload(const T& x, const expr<R, E>& y) { \
  return expr<typename resolve_binary<op_type, T, R>::return_type, \
              wrap::binary<op_type, expr<T, T>, expr<R, E> > > \
    (wrap::binary<op_type, expr<T, T>, expr<R, E> >(expr<T, T>(x), y)); }

SPUTSOFT_MATH_NUMBERS_DETAIL_BINARY_OPS(operator+, ops::binary::add)
SPUTSOFT_MATH_NUMBERS_DETAIL_BINARY_OPS(operator-, ops::binary::subtract)
SPUTSOFT_MATH_NUMBERS_DETAIL_BINARY_OPS(operator*, ops::binary::multiply)
SPUTSOFT_MATH_NUMBERS_DETAIL_BINARY_OPS(operator/, ops::binary::divide)
SPUTSOFT_MATH_NUMBERS_DETAIL_BINARY_OPS(operator%, ops::binary::remainder)

} // namespace detail
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_EXPRESSIONS_HPP_

