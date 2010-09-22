/* 
 * File:   sputsoft/numbers/detail/named_ops.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-04-30 15:04Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP

//#include <cstring>
#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/common_type.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

namespace ops {
  namespace binary {
    struct add {};
    struct sub {};
    struct mul {};
    struct div {};
    struct div_floor {};
    struct div_ceil {};
    struct div_trunc {};
    struct rem {};
    struct rem_floor {};
    struct rem_ceil {};
    struct rem_trunc {};
    struct quotrem {};
    struct quotrem_floor {};
    struct quotrem_ceil {};
    struct quotrem_trunc {};
    struct bit_and {};
    struct bit_and_not {};
    struct bit_or {};
    struct bit_xor {};
    struct lshift {};
    struct rshift {};
  }
  namespace unary {
    struct identity {};
    struct negate {};
    struct abs {};
    struct bit_not {};
  }
}

template <typename Op, typename X, typename Y>
struct binary_result : public sputsoft::numbers::common_type<X, Y> {};

template <typename Op, typename T>
struct unary_result {};

// evaluators

/*template <typename R, typename Forw> struct set_4_eval;
template <typename V1, typename V2>                         struct cmp_r2_eval;
template <typename V1, typename V2>                         struct equal_r2_eval;
template <typename T>                                       struct is_zero_r1_eval;
template <typename T>                                       struct is_positive_r1_eval;
template <typename T>                                       struct is_negative_r1_eval;
template <typename R, typename V>                           struct lshift_3_eval;
template <typename R, typename V>                           struct rshift_3_eval;
template <typename Op, typename Q, typename R, typename V1, typename V2> struct evaluator_rrvv;*/

template <typename T> struct floor_log2_eval;

template <typename Op, typename R, typename V1, typename V2> struct evaluator_rvv;
template <typename Op, typename R, typename V> struct evaluator_rv;

template <typename Op, typename Q, typename V1, typename V2> struct function_rvv;

template <typename Op, typename R, typename V1, typename V2> struct function_rt_vv_default;

template <typename Op, typename R, typename V1, typename V2>
struct function_rt_vv : public function_rt_vv_default<Op, R, V1, V2> {};

template <typename Op, typename V1, typename V2>
struct function_vv
  : public function_rt_vv<Op, typename binary_result<Op, V1, V2>::type, V1, V2> {};

template <typename Op, typename V> struct function_v;

template <typename Op, typename R, typename V>
struct enabler_rv {};

template <bool C, typename T> struct enable_if {};
template <typename T> struct enable_if<true, T> { typedef T type; };

template <typename R, typename V>
struct enabler_rv<ops::unary::identity, R, V>
  : public enable_if<(detail::type_rank<R>::value >= detail::type_rank<V>::value)
                      && (is_signed<R>::value || !is_signed<V>::value), void> {};

template <typename Op, typename R, typename V1, typename V2>
struct enabler_rvv
  : public enabler_rv<ops::unary::identity, R, typename binary_result<Op, V1, V2>::type> {};

} // namespace detail

// convenience method
/*template <typename T>
struct eval_type {
  typedef typename detail::resolve_unary<detail::ops::unary::identity, T>::return_type type;
};

template <typename T>
inline typename eval_type<T>::type eval(const T& v) {
  return detail::function_v<detail::ops::unary::identity, T>()(v);
}*/

/*template <typename R, typename Forw>
inline void set(R& r, Forw first, const Forw last, unsigned base=10) {
  detail::set_4_eval<R, Forw>::set(r, first, last, base);
}

template <typename R>
inline void set(R& r, const std::string& st, unsigned base=10) {
  set(r, st.begin(), st.end(), base);
}

template <typename R>
inline void set(R& r, const char* st, unsigned base=10) {
  set(r, st, st + std::strlen(st), base);
}*/

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::identity, R, V>::type
set(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::identity, R, V>()(r, v);
}

/*template <typename R, typename V>
inline void negate(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::negate, R, V>()(r, v);
}

template <typename R, typename V>
inline void abs(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::abs, R, V>()(r, v);
}*/

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::add, R, V1, V2>::type
add(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::add, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::sub, R, V1, V2>::type
sub(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::sub, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::mul, R, V1, V2>::type
mul(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::mul, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::div, R, V1, V2>::type
div(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::div, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::rem, R, V1, V2>::type
rem(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::rem, R, V1, V2>()(r, v1, v2);
}

/*
template <typename R, typename V1, typename V2>
inline void div_floor(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::div_floor, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div_ceil(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::div_ceil, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div_trunc(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::div_trunc, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem_floor(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::rem_floor, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem_ceil(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::rem_ceil, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem_trunc(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::rem_trunc, R, V1, V2>()(r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rrvv<detail::ops::binary::quotrem, Q, R, V1, V2>()(q, r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem_floor(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rrvv<detail::ops::binary::quotrem_floor, Q, R, V1, V2>()(q, r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem_ceil(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rrvv<detail::ops::binary::quotrem_ceil, Q, R, V1, V2>()(q, r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem_trunc(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rrvv<detail::ops::binary::quotrem_trunc, Q, R, V1, V2>()(q, r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_and(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::bit_and, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_or(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::bit_or, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_xor(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::bit_xor, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_and_not(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::bit_and_not, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V>
inline void bitwise_not(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::bit_not, R, V>()(r, v);
}

template <typename R, typename V1>
inline void bit_shift_left(R& r, const V1& v1, std::ptrdiff_t count) {
  detail::lshift_3_eval<R, V1>::lshift(r, v1, count);
}

template <typename R, typename V1>
inline void bit_shift_right(R& r, const V1& v1, std::ptrdiff_t count) {
  detail::rshift_3_eval<R, V1>::rshift(r, v1, count);
}*/

// returns result by value

/*
template <typename Op, typename T>
inline typename detail::resolve_unary<detail::ops::unary::negate, T>::return_type
negate(const T& v) {
  return detail::function_v<detail::ops::unary::negate, T>()(v);
}

template <typename T>
inline typename detail::resolve_unary<detail::ops::unary::abs, T>::return_type
abs(const T& v) {
  return detail::function_v<detail::ops::unary::abs, T>()(v);
}

template <typename T>
inline typename detail::resolve_unary<detail::ops::unary::bit_not, T>::return_type
bitwise_not(const T& v) {
  return detail::function_v<detail::ops::unary::bit_not, T>()(v);
}

template <typename Q, typename V1, typename V2>
inline typename detail::resolve_binary<detail::ops::binary::rem, V1, V2>::return_type
quotrem(Q& q, const V1& v1, const V2& v2) {
  return detail::function_rvv<detail::ops::binary::quotrem, Q, V1, V2>()(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline typename detail::resolve_binary<detail::ops::binary::rem_floor, V1, V2>::return_type
quotrem_floor(Q& q, const V1& v1, const V2& v2) {
  return detail::function_rvv<detail::ops::binary::quotrem_floor, Q, V1, V2>()(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline typename detail::resolve_binary<detail::ops::binary::rem_ceil, V1, V2>::return_type
quotrem_ceil(Q& q, const V1& v1, const V2& v2) {
  return detail::function_rvv<detail::ops::binary::quotrem_ceil, Q, V1, V2>()(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline typename detail::resolve_binary<detail::ops::binary::rem_trunc, V1, V2>::return_type
quotrem_trunc(Q& q, const V1& v1, const V2& v2) {
  return detail::function_rvv<detail::ops::binary::quotrem_trunc, Q, V1, V2>()(q, v1, v2);
}*/

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::add, V1, V2>::type
add(const V1& v1, const V2& v2) {
  return detail::function_vv<detail::ops::binary::add, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::sub, V1, V2>::type
sub(const V1& v1, const V2& v2) {
  return detail::function_vv<detail::ops::binary::sub, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::mul, V1, V2>::type
mul(const V1& v1, const V2& v2) {
  return detail::function_vv<detail::ops::binary::mul, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::div, V1, V2>::type
div(const V1& v1, const V2& v2) {
  return detail::function_vv<detail::ops::binary::div, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::rem, V1, V2>::type
rem(const V1& v1, const V2& v2) {
  return detail::function_vv<detail::ops::binary::rem, V1, V2>()(v1, v2);
}

/*
BINARY_RESULT_RETURNER(div_floor, div_floor)
BINARY_RESULT_RETURNER(div_ceil, div_ceil)
BINARY_RESULT_RETURNER(div_trunc, div_trunc)
BINARY_RESULT_RETURNER(rem_floor, rem_floor)
BINARY_RESULT_RETURNER(rem_ceil, rem_ceil)
BINARY_RESULT_RETURNER(rem_trunc, rem_trunc)
BINARY_RESULT_RETURNER(bitwise_and, bit_and)
BINARY_RESULT_RETURNER(bitwise_or, bit_or)
BINARY_RESULT_RETURNER(bitwise_xor, bit_xor)
BINARY_RESULT_RETURNER(bitwise_and_not, bit_and_not)
BINARY_RESULT_RETURNER(bit_shift_left, lshift)
BINARY_RESULT_RETURNER(bit_shift_right, rshift)
*/
// other

template <typename T>
inline std::size_t floor_log2(T n) {
  return detail::floor_log2_eval<T>()(n);
}

/*
template <typename V1, typename V2>
inline int compare(const V1& v1, const V2& v2) {
  return detail::cmp_r2_eval<V1, V2>::cmp(v1, v2);
}

template <typename V1, typename V2>
inline bool equal(const V1& v1, const V2& v2) {
  return detail::equal_r2_eval<V1, V2>::equal(v1, v2);
}

template <typename T>
inline bool is_zero(const T& v) {
  return detail::is_zero_r1_eval<T>::is_zero(v);
}

template <typename T>
inline bool is_positive(const T& v) {
  return detail::is_positive_r1_eval<T>::is_positive(v);
}

template <typename T>
inline bool is_negative(const T& v) {
  return detail::is_negative_r1_eval<T>::is_negative(v);
}
*/

} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
