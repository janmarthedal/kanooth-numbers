/* 
 * File:   expr.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:06 PM
 */

#ifndef _EXPR_HPP
#define	_EXPR_HPP

#include <sputsoft/numbers/detail/method_arith.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R, typename E=R> class expr;

template <typename R, typename V>
class evaluator_ref1_arg1<expr<R, R>, V> {
public:
  static void set(expr<R, R>& r, const V& v) {
    expr<R, R>::set(r, v);
  }
};

template <typename R, typename V1, typename V2>
class evaluator_ref1_arg2<expr<R, R>, V1, V2> {
public:
  static void add(expr<R, R>& r, const V1& v1, const V2& v2) {
    expr<R, R>::add(r, v1, v2);
  }
  static void sub(expr<R, R>& r, const V1& v1, const V2& v2) {
    expr<R, R>::sub(r, v1, v2);
  }
  static void mul(expr<R, R>& r, const V1& v1, const V2& v2) {
    expr<R, R>::mul(r, v1, v2);
  }
};

} // namespace sputsoft
} // namespace detail
} // namespace numbers

#endif	/* _EXPR_HPP */

