/* 
 * File:   method_arith.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:04 PM
 */

#ifndef _METHOD_ARITH_HPP
#define	_METHOD_ARITH_HPP

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R, typename V1, typename V2>
class evaluator_ref1_arg2;
template <typename R, typename V>
class evaluator_ref1_arg1;

} // namespace sputsoft

template <typename R, typename V1, typename V2>
void add(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_ref1_arg2<R, V1, V2>::add(r, v1, v2);
}

template <typename R, typename V>
void set(R& r, const V& v) {
  detail::evaluator_ref1_arg1<R, V>::set(r, v);
}

} // namespace detail
} // namespace numbers

#endif	/* _METHOD_ARITH_HPP */

