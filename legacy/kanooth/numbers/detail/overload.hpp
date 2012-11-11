/* 
 * File:   kanooth/numbers/detail/overload.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-04-30 15:21Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_OVERLOAD_HPP
#define _KANOOTH_NUMBERS_DETAIL_OVERLOAD_HPP

#ifdef OVERLOAD_EXPRESSION_OPTIMIZE

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/detail/named_ops.hpp>

namespace kanooth {
namespace numbers {
namespace detail {

// Local stuff
namespace {

  template <typename R, typename E> class expr;
  template <typename Op, typename X, typename Y> struct binary;
  template <typename Op, typename X> struct unary;

  template <class T>
  struct resolve_ref {
    typedef T ref_type;
  };

  template <class T>
  struct resolve_ref<numb<T> > {
    typedef const numb<T>& ref_type;
  };

  template <typename T> struct is_numb_or_expr : public kanooth::false_type {};
  template <typename N> struct is_numb_or_expr<numb<N> > : public kanooth::true_type {};
  template <typename R, typename E> struct is_numb_or_expr<expr<R, E> >
    : public kanooth::true_type {};

  template <typename Op, typename T1, typename T2, bool Ok>
  struct approve_binop_overload2 {};

  template <typename Op, typename T1, typename T2>
  struct approve_binop_overload2<Op, T1, T2, true> {
    typedef expr<typename binary_result<Op, T1, T2>::type, binary<Op, T1, T2> > return_type;
    return_type operator()(const T1& x, const T2& y) {
      return return_type(x, y);
    }
  };

  template <typename T1, typename T2>
  struct approve_binop_args
    : public integral_constant<bool, (is_numb_or_expr<T1>::value &&
                                       (is_numb_or_expr<T2>::value || is_native_int<T2>::value)) ||
                                     (is_numb_or_expr<T2>::value &&
                                       (is_numb_or_expr<T1>::value || is_native_int<T1>::value))> {};

  template <typename Op, typename T1, typename T2>
  struct approve_binop_overload
    : approve_binop_overload2<Op, T1, T2, approve_binop_args<T1, T2>::value> {};

  template <typename Op, typename T1, typename T2>
  struct approve_shift_overload
    : approve_binop_overload2<Op, T1, T2, is_numb_or_expr<T1>::value && is_native_int<T2>::value> {};

  template <typename Op, typename T, bool Ok>
  struct approve_unop_overload2 {};

  template <typename Op, typename T>
  struct approve_unop_overload2<Op, T, true> {
    typedef expr<typename unary_result<Op, T>::type, unary<Op, T> > return_type;
    return_type operator()(const T& x) {
      return return_type(x);
    }
  };

  template <typename Op, typename T>
  struct approve_unop_overload
    : approve_unop_overload2<Op, T, is_numb_or_expr<T>::value> {};

  template <typename T1, typename T2>
  struct approve_cmp_overload : public type_if<approve_binop_args<T1, T2>::value, bool> {};

  /*
  template <typename R, typename R2, typename X, typename Y>
  struct expr<R, binary<ops::binary::add, X, expr<R2, unary<ops::unary::negate, Y> > > > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
    expr(const X& _x, const expr<R2, unary<ops::unary::negate, Y> >& _y) : x(_x), y(_y.x) {}
    inline void assign(R& r) const { kanooth::numbers::sub(r, x, y); }
    inline operator R() const { return kanooth::numbers::sub(x, y); }
  };
  */

  template <typename R, typename X, typename Y>
  struct expr<R, binary<ops::binary::add, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::add(r, x, y); }
    inline operator R() const { return kanooth::numbers::add(x, y); }
  };

  template <typename R, typename X, typename Y>
  class expr<R, binary<ops::binary::sub, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
  public:
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::sub(r, x, y); }
    inline operator R() const { return kanooth::numbers::sub(x, y); }
  };

  template <typename R, typename X, typename Y>
  class expr<R, binary<ops::binary::mul, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
  public:
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::mul(r, x, y); }
    inline operator R() const { return kanooth::numbers::mul(x, y); }
  };

  template <typename R, typename X, typename Y>
  class expr<R, binary<ops::binary::div, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
  public:
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::div(r, x, y); }
    inline operator R() const { return kanooth::numbers::div(x, y); }
  };

  template <typename R, typename X, typename Y>
  class expr<R, binary<ops::binary::rem, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
  public:
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::rem(r, x, y); }
    inline operator R() const { return kanooth::numbers::rem(x, y); }
  };

  template <typename R, typename X, typename Y>
  class expr<R, binary<ops::binary::shift_left, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
  public:
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::bit_shift_left(r, x, y); }
    inline operator R() const { return kanooth::numbers::bit_shift_left(x, y); }
  };

  template <typename R, typename X, typename Y>
  class expr<R, binary<ops::binary::shift_right, X, Y> > {
    typename resolve_ref<X>::ref_type x;
    typename resolve_ref<Y>::ref_type y;
  public:
    expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
    inline void assign(R& r) const { kanooth::numbers::bit_shift_right(r, x, y); }
    inline operator R() const { return kanooth::numbers::bit_shift_right(x, y); }
  };

  template <typename R, typename X>
  struct expr<R, unary<ops::unary::negate, X> > {
    typename resolve_ref<X>::ref_type x;
    expr(const X& _x) : x(_x) {}
    inline void assign(R& r) const { kanooth::numbers::negate(r, x); }
    inline operator R() const { return kanooth::numbers::negate(x); }
  };

} // local namespace

} // namespace detail

template <typename R, typename E>
struct eval_result<detail::expr<R, E> > : public eval_result<R> {};

template <typename R1, typename R2, typename E2>
struct is_assignable<R1, detail::expr<R2, E2> > : public is_assignable<R1, R2> {};

} // namespace numbers

template <typename R, typename E>
struct is_integral<numbers::detail::expr<R, E> > : public is_integral<R> {};

namespace numbers {
namespace detail {

template <typename R, typename E>
struct function_v<ops::unary::identity, expr<R, E> > {
  R operator()(const expr<R, E>& e) const {
    return (R) e;
  }
};

template <typename R1, typename R2, typename E2>
struct evaluator_rv<ops::unary::identity, numb<R1>, expr<R2, E2> > {
  void operator()(numb<R1>& r, const expr<R2, E2>& v) const {
    v.assign(r);
  }
};

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::add, T1, T2>::return_type
operator+(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::add, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::sub, T1, T2>::return_type
operator-(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::sub, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::mul, T1, T2>::return_type
operator*(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::mul, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::div, T1, T2>::return_type
operator/(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::div, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::rem, T1, T2>::return_type
operator%(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::rem, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_shift_overload<ops::binary::shift_left, T1, T2>::return_type
operator<<(const T1& x, const T2& y) {
  return approve_shift_overload<ops::binary::shift_left, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_shift_overload<ops::binary::shift_right, T1, T2>::return_type
operator>>(const T1& x, const T2& y) {
  return approve_shift_overload<ops::binary::shift_right, T1, T2>()(x, y);
}

template <typename T>
typename approve_unop_overload<ops::unary::negate, T>::return_type
operator-(const T& x) {
  return approve_unop_overload<ops::unary::negate, T>()(x);
}


template <typename T1, typename T2>
inline typename approve_cmp_overload<T1, T2>::type
operator==(const T1& x, const T2& y) {
  return kanooth::numbers::is_equal(kanooth::numbers::eval(x), kanooth::numbers::eval(y));
}

template <typename T1, typename T2>
inline typename approve_cmp_overload<T1, T2>::type
operator!=(const T1& x, const T2& y) {
  return kanooth::numbers::is_not_equal(kanooth::numbers::eval(x), kanooth::numbers::eval(y));
}

template <typename T1, typename T2>
inline typename approve_cmp_overload<T1, T2>::type
operator<(const T1& x, const T2& y) {
  return kanooth::numbers::is_less(kanooth::numbers::eval(x), kanooth::numbers::eval(y));
}

template <typename T1, typename T2>
inline typename approve_cmp_overload<T1, T2>::type
operator>(const T1& x, const T2& y) {
  return kanooth::numbers::is_greater(kanooth::numbers::eval(x), kanooth::numbers::eval(y));
}

template <typename T1, typename T2>
inline typename approve_cmp_overload<T1, T2>::type
operator<=(const T1& x, const T2& y) {
  return kanooth::numbers::is_less_or_equal(kanooth::numbers::eval(x), kanooth::numbers::eval(y));
}

template <typename T1, typename T2>
inline typename approve_cmp_overload<T1, T2>::type
operator>=(const T1& x, const T2& y) {
  return kanooth::numbers::is_greater_or_equal(kanooth::numbers::eval(x), kanooth::numbers::eval(y));
}


template <typename N, typename T>
inline numb<N>& operator+=(numb<N>& x, const T& y) {
  return x = x + y;
}

template <typename N, typename T>
inline numb<N>& operator-=(numb<N>& x, const T& y) {
  return x = x - y;
}

template <typename N, typename T>
inline numb<N>& operator*=(numb<N>& x, const T& y) {
  return x = x * y;
}

template <typename N, typename T>
inline numb<N>& operator/=(numb<N>& x, const T& y) {
  return x = x / y;
}

template <typename N, typename T>
inline numb<N>& operator%=(numb<N>& x, const T& y) {
  return x = x % y;
}

template <typename N, typename T>
inline numb<N>& operator<<=(numb<N>& x, const T& y) {
  return x = x << y;
}

template <typename N, typename T>
inline numb<N>& operator>>=(numb<N>& x, const T& y) {
  return x = x >> y;
}

template <typename R, typename E>
std::ostream& operator<<(std::ostream& os, const expr<R, E>& e) {
  return os << (R) e;
}

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // OVERLOAD_EXPRESSION_OPTIMIZE

#endif // _KANOOTH_NUMBERS_DETAIL_OVERLOAD_HPP