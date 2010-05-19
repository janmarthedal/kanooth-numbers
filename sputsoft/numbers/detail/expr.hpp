/* 
 * File:   sputsoft/numbers/detail/expr.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-04-30 15:06
 *
 * $Id$
 */

#ifndef _EXPR_HPP
#define	_EXPR_HPP

#include <sputsoft/numbers/detail/named_ops.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R, typename E=R> class expr;
template <typename Op, typename X, typename Y> struct resolve_binary;

template <typename Op, typename R, typename E1, typename E2>
struct resolve_binary<Op, expr<R, E1>, expr<R, E2> > {
  typedef R return_type;
};

template <class T>
struct resolve_ref {
  typedef T ref_type;
};

/*template <class R>
struct resolve_ref<expr<R> > {
  typedef const expr<R>& ref_type;
};*/

namespace ops {
  namespace binary {
    class add {};
    class subtract {};
    class multiply {};
    class divide {};
    class remainder {};
    class lshift {};
    class rshift {};
  }
  namespace unary {
    class negate {};
  }
}

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

/* Named operator specializations for expr-types */

template <typename R, typename V>
struct set_2_eval<expr<R>, V> {
  static void set(expr<R>& r, const V& v) {
    r.set(v);
  }
};

template <typename R, typename V>
struct negate_2_eval<expr<R>, V> {
  static void negate(expr<R>& r, const V& v) {
    r.negate(v);
  }
};

template <typename R, typename V1, typename V2>
struct add_3_eval<expr<R>, V1, V2> {
  static void add(expr<R>& r, const V1& v1, const V2& v2) {
    r.add(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct sub_3_eval<expr<R>, V1, V2> {
  static void sub(expr<R>& r, const V1& v1, const V2& v2) {
    r.sub(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct mul_3_eval<expr<R>, V1, V2> {
  static void mul(expr<R>& r, const V1& v1, const V2& v2) {
    r.mul(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct div_3_eval<expr<R>, V1, V2> {
  static void div(expr<R>& r, const V1& v1, const V2& v2) {
    r.div(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct div_floor_3_eval<expr<R>, V1, V2> {
  static void div_floor(expr<R>& r, const V1& v1, const V2& v2) {
    r.div_floor(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct div_ceil_3_eval<expr<R>, V1, V2> {
  static void div_ceil(expr<R>& r, const V1& v1, const V2& v2) {
    r.div_ceil(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct div_trunc_3_eval<expr<R>, V1, V2> {
  static void div_trunc(expr<R>& r, const V1& v1, const V2& v2) {
    r.div_trunc(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct rem_3_eval<expr<R>, V1, V2> {
  static void rem(expr<R>& r, const V1& v1, const V2& v2) {
    r.rem(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct rem_floor_3_eval<expr<R>, V1, V2> {
  static void rem_floor(expr<R>& r, const V1& v1, const V2& v2) {
    r.rem_floor(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct rem_ceil_3_eval<expr<R>, V1, V2> {
  static void rem_ceil(expr<R>& r, const V1& v1, const V2& v2) {
    r.rem_ceil(v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct rem_trunc_3_eval<expr<R>, V1, V2> {
  static void rem_trunc(expr<R>& r, const V1& v1, const V2& v2) {
    r.rem_trunc(v1, v2);
  }
};

template <typename R, typename V2>
struct rem_r2_eval<expr<R>, V2> {
  static V2 rem(const expr<R>& v1, const V2& v2) {
    return expr<R>::rem(v1, v2);
  }
};

template <typename R, typename V2>
struct rem_floor_r2_eval<expr<R>, V2> {
  static V2 rem_floor(const expr<R>& v1, const V2& v2) {
    return expr<R>::rem_floor(v1, v2);
  }
};

template <typename R, typename V2>
struct rem_ceil_r2_eval<expr<R>, V2> {
  static V2 rem_ceil(const expr<R>& v1, const V2& v2) {
    return expr<R>::rem_ceil(v1, v2);
  }
};

template <typename R, typename V2>
struct rem_trunc_r2_eval<expr<R>, V2> {
  static V2 rem_trunc(const expr<R>& v1, const V2& v2) {
    return expr<R>::rem_trunc(v1, v2);
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct quotrem_4_eval<expr<Q, Q>, R, V1, V2> {
  static void quotrem(expr<Q, Q>& q, R& r, const V1& v1, const V2& v2) {
    expr<Q, Q>::quotrem(q, r, v1, v2);
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct quotrem_floor_4_eval<expr<Q, Q>, R, V1, V2> {
  static void quotrem_floor(expr<Q, Q>& q, R& r, const V1& v1, const V2& v2) {
    expr<Q, Q>::quotrem_floor(q, r, v1, v2);
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct quotrem_ceil_4_eval<expr<Q, Q>, R, V1, V2> {
  static void quotrem_ceil(expr<Q, Q>& q, R& r, const V1& v1, const V2& v2) {
    expr<Q, Q>::quotrem_ceil(q, r, v1, v2);
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct quotrem_trunc_4_eval<expr<Q, Q>, R, V1, V2> {
  static void quotrem_trunc(expr<Q, Q>& q, R& r, const V1& v1, const V2& v2) {
    expr<Q, Q>::quotrem_trunc(q, r, v1, v2);
  }
};

template <typename Q, typename V1, typename V2>
struct quotrem_r3_eval<expr<Q, Q>, V1, V2> {
  static V2 quotrem(expr<Q, Q>& q, const V1& v1, const V2& v2) {
    return q.quotrem(v1, v2);
  }
};

template <typename Q, typename V1, typename V2>
struct quotrem_floor_r3_eval<expr<Q, Q>, V1, V2> {
  static V2 quotrem_floor(expr<Q, Q>& q, const V1& v1, const V2& v2) {
    return q.quotrem_floor(v1, v2);
  }
};

template <typename Q, typename V1, typename V2>
struct quotrem_ceil_r3_eval<expr<Q, Q>, V1, V2> {
  static V2 quotrem_ceil(expr<Q, Q>& q, const V1& v1, const V2& v2) {
    return q.quotrem_ceil(v1, v2);
  }
};

template <typename Q, typename V1, typename V2>
struct quotrem_trunc_r3_eval<expr<Q, Q>, V1, V2> {
  static V2 quotrem_trunc(expr<Q, Q>& q, const V1& v1, const V2& v2) {
    return q.quotrem_trunc(v1, v2);
  }
};

template <typename R>
struct cmp_r2_eval<expr<R>, expr<R> > {
  static int cmp(const expr<R>& v1, const expr<R>& v2) {
    return v1.cmp(v2);
  }
};

template <typename R, typename V>
struct cmp_r2_eval<expr<R>, V> {
  static int cmp(const expr<R>& v1, const V& v2) {
    return v1.cmp(v2);
  }
};

template <typename R, typename V>
struct cmp_r2_eval<V, expr<R> > {
  static int cmp(const V& v1, const expr<R>& v2) {
    return -v2.cmp(v1);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _EXPR_HPP */

