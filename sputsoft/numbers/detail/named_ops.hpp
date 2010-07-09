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

#include <cstring>
#include <sputsoft/detail/types.hpp>
#include <sputsoft/numbers/detail/number_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

/* Operator names, used when overloading and for resolving return types */

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

template <typename Op, typename X, typename Y> struct resolve_binary;
template <typename Op, typename T>
struct resolve_unary {
  typedef T return_type;
};

// evaluators

template <typename R, typename Forw>                        struct set_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_4_eval;
template <typename R, typename V1, typename V2>             struct div_floor_3_eval;
template <typename R, typename V1, typename V2>             struct div_ceil_3_eval;
template <typename R, typename V1, typename V2>             struct div_trunc_3_eval;
template <typename R, typename V1, typename V2>             struct rem_floor_3_eval;
template <typename R, typename V1, typename V2>             struct rem_ceil_3_eval;
template <typename R, typename V1, typename V2>             struct rem_trunc_3_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_floor_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_ceil_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_trunc_4_eval;
template <typename R, typename V1, typename V2>             struct or_3_eval;
template <typename R, typename V1, typename V2>             struct xor_3_eval;
template <typename R, typename V1, typename V2>             struct and_not_3_eval;
template <typename R, typename V>                           struct not_2_eval;
template <typename V1, typename V2>                         struct cmp_r2_eval;
template <typename V1, typename V2>                         struct equal_r2_eval;
template <typename T>                                       struct is_zero_r1_eval;
template <typename T>                                       struct is_positive_r1_eval;
template <typename T>                                       struct is_negative_r1_eval;
template <typename T>                                       struct log2_floor_evaluator;
template <typename R, typename V>                           struct lshift_3_eval;
template <typename R, typename V>                           struct rshift_3_eval;

template <typename Op, typename R, typename V1, typename V2> struct evaluator_rvv;
template <typename Op, typename R, typename V> struct evaluator_rv;
template <typename Op, typename V1, typename V2> struct function_vv;
template <typename Op, typename V> struct function_v;

} // namespace detail

template <typename R, typename Forw>
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
}

template <typename R, typename V>
inline void set(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::identity, R, V>()(r, v);
}

/* Unary negate */

template <typename R, typename V>
inline void negate(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::negate, R, V>()(r, v);
}

template <typename T>
typename detail::resolve_unary<detail::ops::unary::negate, T>::return_type negate(const T& v) {
  return detail::function_v<detail::ops::unary::negate, T>()(v);
}

/* Unary abs */

template <typename R, typename V>
inline void abs(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::abs, R, V>()(r, v);
}

template <typename T>
typename detail::resolve_unary<detail::ops::unary::abs, T>::return_type abs(const T& v) {
  return detail::function_v<detail::ops::unary::abs, T>()(v);
}

template <typename R, typename V1, typename V2>
inline void add(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::add, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void sub(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::sub, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void mul(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::mul, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::div, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div_floor(R& r, const V1& v1, const V2& v2) {
  detail::div_floor_3_eval<R, V1, V2>::div_floor(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div_ceil(R& r, const V1& v1, const V2& v2) {
  detail::div_ceil_3_eval<R, V1, V2>::div_ceil(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div_trunc(R& r, const V1& v1, const V2& v2) {
  detail::div_trunc_3_eval<R, V1, V2>::div_trunc(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::rem, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem_floor(R& r, const V1& v1, const V2& v2) {
  detail::rem_floor_3_eval<R, V1, V2>::rem_floor(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem_ceil(R& r, const V1& v1, const V2& v2) {
  detail::rem_ceil_3_eval<R, V1, V2>::rem_ceil(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void rem_trunc(R& r, const V1& v1, const V2& v2) {
  detail::rem_trunc_3_eval<R, V1, V2>::rem_trunc(r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::quotrem_4_eval<Q, R, V1, V2>::quotrem(q, r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem_floor(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::quotrem_floor_4_eval<Q, R, V1, V2>::quotrem_floor(q, r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem_ceil(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::quotrem_ceil_4_eval<Q, R, V1, V2>::quotrem_ceil(q, r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem_trunc(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::quotrem_trunc_4_eval<Q, R, V1, V2>::quotrem_trunc(q, r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_and(R& r, const V1& v1, const V2& v2) {
  detail::evaluator_rvv<detail::ops::binary::bit_and, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_or(R& r, const V1& v1, const V2& v2) {
  detail::or_3_eval<R, V1, V2>::bit_or(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_xor(R& r, const V1& v1, const V2& v2) {
  detail::xor_3_eval<R, V1, V2>::bit_xor(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void bitwise_and_not(R& r, const V1& v1, const V2& v2) {
  detail::and_not_3_eval<R, V1, V2>::bit_and_not(r, v1, v2);
}

template <typename R, typename V>
inline void bitwise_not(R& r, const V& v) {
  detail::not_2_eval<R, V>::bit_not(r, v);
}

template <typename R, typename V1>
inline void bit_shift_left(R& r, const V1& v1, std::ptrdiff_t count) {
  detail::lshift_3_eval<R, V1>::lshift(r, v1, count);
}

template <typename R, typename V1>
inline void bit_shift_right(R& r, const V1& v1, std::ptrdiff_t count) {
  detail::rshift_3_eval<R, V1>::rshift(r, v1, count);
}

// returns result by value

template <typename Q, typename V1, typename V2>
typename detail::resolve_binary<detail::ops::binary::rem, V1, V2>::return_type
quotrem(Q& q, const V1& v1, const V2& v2) {
  typename detail::resolve_binary<detail::ops::binary::rem, V1, V2>::return_type r;
  quotrem(q, r, v1, v2);
  return r;
}

template <typename Q, typename V1, typename V2>
typename detail::resolve_binary<detail::ops::binary::rem_floor, V1, V2>::return_type
quotrem_floor(Q& q, const V1& v1, const V2& v2) {
  typename detail::resolve_binary<detail::ops::binary::rem_floor, V1, V2>::return_type r;
  quotrem_floor(q, r, v1, v2);
  return r;
}

template <typename Q, typename V1, typename V2>
typename detail::resolve_binary<detail::ops::binary::rem_ceil, V1, V2>::return_type
quotrem_ceil(Q& q, const V1& v1, const V2& v2) {
  typename detail::resolve_binary<detail::ops::binary::rem_ceil, V1, V2>::return_type r;
  quotrem_ceil(q, r, v1, v2);
  return r;
}

template <typename Q, typename V1, typename V2>
typename detail::resolve_binary<detail::ops::binary::rem_trunc, V1, V2>::return_type
quotrem_trunc(Q& q, const V1& v1, const V2& v2) {
  typename detail::resolve_binary<detail::ops::binary::rem_trunc, V1, V2>::return_type r;
  quotrem_trunc(q, r, v1, v2);
  return r;
}

#define BINARY_RESULT_RETURNER(NAME, OP) \
template <typename V1, typename V2> \
typename detail::resolve_binary<detail::ops::binary::OP, V1, V2>::return_type \
NAME(const V1& v1, const V2& v2) { \
  return detail::function_vv<detail::ops::binary::OP, V1, V2>()(v1, v2); \
}

BINARY_RESULT_RETURNER(add, add)
BINARY_RESULT_RETURNER(sub, sub)
BINARY_RESULT_RETURNER(mul, mul)
BINARY_RESULT_RETURNER(div, div)
BINARY_RESULT_RETURNER(div_floor, div_floor)
BINARY_RESULT_RETURNER(div_ceil, div_ceil)
BINARY_RESULT_RETURNER(div_trunc, div_trunc)
BINARY_RESULT_RETURNER(rem, rem)
BINARY_RESULT_RETURNER(rem_floor, rem_floor)
BINARY_RESULT_RETURNER(rem_ceil, rem_ceil)
BINARY_RESULT_RETURNER(rem_trunc, rem_trunc)
BINARY_RESULT_RETURNER(bitwise_and, bit_and)
BINARY_RESULT_RETURNER(bitwise_or, bit_or)
BINARY_RESULT_RETURNER(bitwise_xor, bit_xor)
BINARY_RESULT_RETURNER(bitwise_and_not, bit_and_not)
BINARY_RESULT_RETURNER(bit_shift_left, lshift)
BINARY_RESULT_RETURNER(bit_shift_right, rshift)

template <typename T>
typename detail::resolve_unary<detail::ops::unary::bit_not, T>::return_type
bitwise_not(const T& v1) {
  typename detail::resolve_unary<detail::ops::unary::bit_not, T>::return_type r;
  bitwise_not(r, v1);
  return r;
}

// other

template <typename T>
inline std::size_t log2_floor(T n) {
  return detail::log2_floor_evaluator<T>::log2_floor(n);
}

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

} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
