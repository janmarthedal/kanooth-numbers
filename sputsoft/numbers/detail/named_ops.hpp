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
#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/number_traits.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

namespace ops {
  namespace binary {
    struct add {};
    struct sub {};
    struct mul {};
    struct div {};
    struct floor_div {};
    struct ceil_div {};
    struct trunc_div {};
    struct rem {};
    struct floor_rem {};
    struct ceil_rem {};
    struct trunc_rem {};
    struct bit_and {};
    struct bit_and_not {};
    struct bit_or {};
    struct bit_xor {};
    struct shift_left {};
    struct shift_right {};
  }
  namespace unary {
    struct identity {};
    struct negate {};
    struct abs {};
    struct floor {};
    struct ceil {};
    struct trunc {};
    struct bit_not {};
  }
  namespace binary_compare {
    struct equal {};
    struct not_equal {};
    struct less {};
    struct greater {};
    struct less_or_equal {};
    struct greater_or_equal {};
  }
}

template <typename Op, typename X, typename Y>
struct binary_result : public sputsoft::numbers::common_type<X, Y> {};

template <typename Op, typename T>
struct unary_result {};

// evaluators

template <typename R, typename Forw> struct set_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct divrem_evaluator;
template <typename V1, typename V2> struct compare_eval;
template <typename Op, typename V1, typename V2> struct bool_compare_eval;
template <typename T> struct is_zero_eval;
template <typename T> struct is_positive_eval;
template <typename T> struct is_negative_eval;
template <typename T> struct floor_log2_eval;
template <typename R, typename V> struct shift_left_eval;

// General evaluators

template <typename Op, typename R, typename V1, typename V2> struct evaluator_rvv;
template <typename Op, typename R, typename V> struct evaluator_rv;

// Enablers

template <typename Op, typename R, typename V>
struct enabler_rv;

template <typename Op, typename R, typename V1, typename V2>
struct enabler_rvv;

// Functions

template <typename Op, typename V> struct function_v;

template <typename Op, typename R, typename V1, typename V2>
struct function_rt_vv;

template <typename Op, typename V1, typename V2>
struct function_vv : public function_rt_vv<Op, typename binary_result<Op, V1, V2>::type, V1, V2> {};

template <typename Q, typename R, typename V1, typename V2>
struct function_divrem;

template <typename Q, typename R, typename V1, typename V2>
struct function_floor_divrem;

} // namespace detail

// convenience method
template <typename T>
struct eval_type : public detail::unary_result<detail::ops::unary::identity, T> {};

template <typename T>
inline typename eval_type<T>::type eval(const T& v) {
  return detail::function_v<detail::ops::unary::identity, T>()(v);
}

// Unary

template <typename R, typename Forw>
inline typename type_if<is_number<R>::value, void>::type
set(R& r, Forw first, const Forw last, unsigned base=10) {
  detail::set_4_eval<R, Forw>()(r, first, last, base);
}

template <typename R>
inline typename type_if<is_number<R>::value, void>::type
set(R& r, const std::string& st, unsigned base) {
  set(r, st.begin(), st.end(), base);
}

template <typename R>
inline typename type_if<is_number<R>::value, void>::type
set(R& r, const char* st, unsigned base=10) {
  set(r, st, st + std::strlen(st), base);
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::identity, R, V>::type
set(R& r, const V& v) {
  detail::evaluator_rv<detail::ops::unary::identity, R, V>()(r, v);
}

namespace {
  template <typename Op, typename R, typename V>
  struct evaluator_rv_help {
    inline void operator()(R& r, const V& v) const {
      detail::evaluator_rv<Op, R, typename eval_type<V>::type>()(r, eval(v));
    }
  };
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::negate, R, V>::type
negate(R& r, const V& v) {
  evaluator_rv_help<detail::ops::unary::negate, R, V>()(r, v);
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::abs, R, V>::type
abs(R& r, const V& v) {
  evaluator_rv_help<detail::ops::unary::abs, R, V>()(r, v);
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::trunc, R, V>::type
trunc(R& r, const V& v) {
  evaluator_rv_help<detail::ops::unary::trunc, R, V>()(r, v);
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::floor, R, V>::type
floor(R& r, const V& v) {
  evaluator_rv_help<detail::ops::unary::floor, R, V>()(r, v);
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::ceil, R, V>::type
ceil(R& r, const V& v) {
  evaluator_rv_help<detail::ops::unary::ceil, R, V>()(r, v);
}

template <typename R, typename V>
inline typename detail::enabler_rv<detail::ops::unary::bit_not, R, V>::type
bitwise_not(R& r, const V& v) {
  evaluator_rv_help<detail::ops::unary::bit_not, R, V>()(r, v);
}

// Binary

namespace {
  template <typename Op, typename R, typename V1, typename V2>
  struct evaluator_rvv_help {
    inline void operator()(R& r, const V1& v1, const V2& v2) const {
      detail::evaluator_rvv<Op, R,
            typename eval_type<V1>::type, typename eval_type<V2>::type>()(r, eval(v1), eval(v2));
    }
  };
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::add, R, V1, V2>::type
add(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::add, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::sub, R, V1, V2>::type
sub(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::sub, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::mul, R, V1, V2>::type
mul(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::mul, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::div, R, V1, V2>::type
div(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::div, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::rem, R, V1, V2>::type
rem(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::rem, R, V1, V2>()(r, v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline typename type_if<
                  detail::enabler_rvv<detail::ops::binary::div, Q, V1, V2>::enabled &&
                  detail::enabler_rvv<detail::ops::binary::rem, R, V1, V2>::enabled, void>::type
divrem(Q& q, R& r, const V1& v1, const V2& v2) {
  return detail::divrem_evaluator<Q, R, V1, V2>()(q, r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::floor_div, R, V1, V2>::type
floor_div(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::floor_div, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::floor_rem, R, V1, V2>::type
floor_rem(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::floor_rem, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::ceil_div, R, V1, V2>::type
ceil_div(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::ceil_div, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::ceil_rem, R, V1, V2>::type
ceil_rem(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::ceil_rem, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::trunc_div, R, V1, V2>::type
trunc_div(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::trunc_div, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::trunc_rem, R, V1, V2>::type
trunc_rem(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::trunc_rem, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::bit_and, R, V1, V2>::type
bitwise_and(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::bit_and, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::bit_or, R, V1, V2>::type
bitwise_or(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::bit_or, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::bit_xor, R, V1, V2>::type
bitwise_xor(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::bit_xor, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline typename detail::enabler_rvv<detail::ops::binary::bit_and_not, R, V1, V2>::type
bitwise_and_not(R& r, const V1& v1, const V2& v2) {
  evaluator_rvv_help<detail::ops::binary::bit_and_not, R, V1, V2>()(r, v1, v2);
}

template <typename R, typename V>
inline typename detail::enabler_rvv<detail::ops::binary::shift_left, R, V, std::ptrdiff_t>::type
bit_shift_left(R& r, const V& v, std::ptrdiff_t count) {
  evaluator_rvv_help<detail::ops::binary::shift_left, R, V, std::ptrdiff_t>()(r, v, count);
}

template <typename R, typename V>
inline typename detail::enabler_rvv<detail::ops::binary::shift_left, R, V, std::ptrdiff_t>::type
bit_shift_right(R& r, const V& v, std::ptrdiff_t count) {
  evaluator_rvv_help<detail::ops::binary::shift_right, R, V, std::ptrdiff_t>()(r, v, count);
}

// returns result by value

namespace {
  template <typename Op, typename V>
  struct function_v_help {
    inline typename detail::unary_result<Op, V>::type operator()(const V& v) const {
      return detail::function_v<Op, typename eval_type<V>::type>()(eval(v));
    }
  };
}

template <typename T>
inline typename detail::unary_result<detail::ops::unary::abs, T>::type
abs(const T& v) {
  return function_v_help<detail::ops::unary::abs, T>()(v);
}

template <typename Op, typename T>
inline typename detail::unary_result<detail::ops::unary::negate, T>::type
negate(const T& v) {
  return function_v_help<detail::ops::unary::negate, T>()(v);
}

template <typename T>
inline typename detail::unary_result<detail::ops::unary::bit_not, T>::type
bitwise_not(const T& v) {
  return function_v_help<detail::ops::unary::bit_not, T>()(v);
}

namespace {
  template <typename Op, typename V1, typename V2>
  struct function_vv_help {
    inline typename detail::binary_result<Op, V1, V2>::type
    operator()(const V1& v1, const V2& v2) const {
      return detail::function_vv<Op, typename eval_type<V1>::type, typename eval_type<V2>::type>()
                (eval(v1), eval(v2));
    }
  };
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::add, V1, V2>::type
add(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::add, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::sub, V1, V2>::type
sub(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::sub, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::mul, V1, V2>::type
mul(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::mul, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::div, V1, V2>::type
div(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::div, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::trunc_div, V1, V2>::type
trunc_div(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::trunc_div, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::floor_div, V1, V2>::type
floor_div(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::floor_div, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::ceil_div, V1, V2>::type
ceil_div(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::ceil_div, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::rem, V1, V2>::type
rem(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::rem, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::trunc_rem, V1, V2>::type
trunc_rem(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::trunc_rem, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::floor_rem, V1, V2>::type
floor_rem(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::floor_rem, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::ceil_rem, V1, V2>::type
ceil_rem(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::ceil_rem, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::bit_and, V1, V2>::type
bitwise_and(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::bit_and, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::bit_or, V1, V2>::type
bitwise_or(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::bit_or, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::bit_xor, V1, V2>::type
bitwise_xor(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::bit_xor, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename detail::binary_result<detail::ops::binary::bit_and_not, V1, V2>::type
bitwise_and_not(const V1& v1, const V2& v2) {
  return function_vv_help<detail::ops::binary::bit_and_not, V1, V2>()(v1, v2);
}

template <typename V>
inline typename detail::binary_result<detail::ops::binary::shift_left, V, std::ptrdiff_t>::type
bit_shift_left(const V& v, std::ptrdiff_t count) {
  return function_vv_help<detail::ops::binary::shift_left, V, std::ptrdiff_t>()(v, count);
}

template <typename V>
inline typename detail::binary_result<detail::ops::binary::shift_left, V, std::ptrdiff_t>::type
bit_shift_right(const V& v, std::ptrdiff_t count) {
  return function_vv_help<detail::ops::binary::shift_right, V, std::ptrdiff_t>()(v, count);
}

template <typename Q, typename V1, typename V2>
inline typename type_if<detail::enabler_rvv<detail::ops::binary::div, Q, V1, V2>::enabled,
                 typename detail::binary_result<detail::ops::binary::rem, V1, V2>::type>::type
divrem(Q& q, const V1& v1, const V2& v2) {
  return detail::function_divrem<Q,
          typename detail::binary_result<detail::ops::binary::rem, V1, V2>::type,
          V1, V2>()(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline typename type_if<detail::enabler_rvv<detail::ops::binary::floor_div, Q, V1, V2>::enabled,
                 typename detail::binary_result<detail::ops::binary::floor_rem, V1, V2>::type>::type
floor_divrem(Q& q, const V1& v1, const V2& v2) {
  return detail::function_floor_divrem<Q,
          typename detail::binary_result<detail::ops::binary::floor_rem, V1, V2>::type,
          V1, V2>()(q, v1, v2);
}

// other

template <typename T>
inline std::size_t floor_log2(T n) {
  return detail::floor_log2_eval<T>()(n);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, int>::type
compare(const V1& v1, const V2& v2) {
  return detail::compare_eval<V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, bool>::type
is_equal(const V1& v1, const V2& v2) {
  return detail::bool_compare_eval<detail::ops::binary_compare::equal, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, bool>::type
is_not_equal(const V1& v1, const V2& v2) {
  return detail::bool_compare_eval<detail::ops::binary_compare::not_equal, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, bool>::type
is_less(const V1& v1, const V2& v2) {
  return detail::bool_compare_eval<detail::ops::binary_compare::less, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, bool>::type
is_greater(const V1& v1, const V2& v2) {
  return detail::bool_compare_eval<detail::ops::binary_compare::greater, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, bool>::type
is_less_or_equal(const V1& v1, const V2& v2) {
  return detail::bool_compare_eval<detail::ops::binary_compare::less_or_equal, V1, V2>()(v1, v2);
}

template <typename V1, typename V2>
inline typename type_if<is_number<V1>::value && is_number<V2>::value, bool>::type
is_greater_or_equal(const V1& v1, const V2& v2) {
  return detail::bool_compare_eval<detail::ops::binary_compare::greater_or_equal, V1, V2>()(v1, v2);
}

template <typename T>
inline typename type_if<is_number<T>::value, bool>::type
is_zero(const T& v) {
  return detail::is_zero_eval<T>()(v);
}

template <typename T>
inline typename type_if<is_number<T>::value, bool>::type
is_positive(const T& v) {
  return detail::is_positive_eval<T>()(v);
}

template <typename T>
inline typename type_if<is_number<T>::value, bool>::type
is_negative(const T& v) {
  return detail::is_negative_eval<T>()(v);
}

} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
