/* 
 * File:   expr.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:06 PM
 *
 * $Id$
 */

#ifndef _EXPR_HPP
#define	_EXPR_HPP

#include <sputsoft/numbers/detail/method_arith.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R, typename E=R> class expr;

template <typename R, typename V>
struct set_2_eval<expr<R, R>, V> {
  static void set(expr<R, R>& r, const V& v) {
    r.set(v);
  }
};

template <typename R, typename V1, typename V2>
struct add_3_eval<expr<R, R>, V1, V2> {
  static void add(expr<R, R>& r, const V1& v1, const V2& v2) {
    r.add(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct sub_3_eval<expr<R, R>, V1, V2> {
  static void sub(expr<R, R>& r, const V1& v1, const V2& v2) {
    r.sub(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct mul_3_eval<expr<R, R>, V1, V2> {
  static void mul(expr<R, R>& r, const V1& v1, const V2& v2) {
    r.mul(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct div_3_eval<expr<R, R>, V1, V2> {
  static void div(expr<R, R>& r, const V1& v1, const V2& v2) {
    r.div(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct rem_3_eval<expr<R, R>, V1, V2> {
  static void rem(expr<R, R>& r, const V1& v1, const V2& v2) {
    r.rem(v1, v2);
  }
};

template <typename R, typename V2>
struct rem_r2_eval<expr<R, R>, V2> {
  static V2 rem(const expr<R, R>& v1, const V2& v2) {
    return v1.rem(v2);
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct quotrem_4_eval<expr<Q, Q>, R, V1, V2> {
  static void rem(expr<Q, Q>& q, R& r, const V1& v1, const V2& v2) {
    expr<Q, Q>::quotrem(q, r, v1, v2);
  }
};

template <typename Q, typename V1, typename V2>
struct quotrem_r3_eval<expr<Q, Q>, V1, V2> {
  static V2 quotrem(expr<Q, Q>& q, const V1& v1, const V2& v2) {
    return q.quotrem(v1, v2);
  }
};

template <typename R, typename V>
struct cmp_r2_eval<expr<R, R>, V> {
  static int cmp(const expr<R, R>& v1, const V& v2) {
    return v1.cmp(v2);
  }
};

template <typename R, typename V>
struct cmp_r2_eval<V, expr<R, R> > {
  static int cmp(const V& v1, const expr<R, R>& v2) {
    return -v2.cmp(v1);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _EXPR_HPP */

