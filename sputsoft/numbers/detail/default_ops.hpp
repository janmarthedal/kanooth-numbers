/* 
 * File:   sputsoft/numbers/detail/default_ops.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-06-01 15:05Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_DEFAULT_OPS_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_DEFAULT_OPS_HPP

#include <sputsoft/numbers/detail/named_ops.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Op, typename T>
struct resolve_binary<Op, T, T> {
  typedef T return_type;
};

/* Unary */

template <typename Op, typename R, typename V>
struct evaluator_rv {
  void operator()(R& r, const V& v) const {
    sputsoft::numbers::set(r, function_v<Op, V>()(v));
  }
};

template <typename Op, typename V>
struct function_v {
  typedef typename resolve_unary<Op, V>::return_type return_type;
  return_type operator()(const V& v) const {
    return_type r;
    evaluator_rv<Op, return_type, V>()(r, v);
    return r;
  }
};

/* Unary identity */

template <typename R, typename V>
struct evaluator_rv<ops::unary::identity, R, V> {
  void operator()(R& r, const V& v) const {
    r = v;
  }
};

/* Unary negate */

template <typename T>
struct resolve_unary<ops::unary::negate, T> {
  typedef typename sputsoft::number_traits<T>::signed_type return_type;
};

template <typename V>
struct function_v<ops::unary::negate, V> {
  typedef typename resolve_unary<ops::unary::negate, V>::return_type return_type;
  return_type operator()(const V& v) { return -((return_type) v); }
};

/* Unary abs */

template <typename T>
struct resolve_unary<ops::unary::abs, T> {
  typedef typename sputsoft::number_traits<T>::unsigned_type return_type;
};

template <typename R, typename V>
struct evaluator_rv<ops::unary::abs, R, V> {
  typedef typename resolve_unary<ops::unary::abs, V>::return_type return_type;
  void operator()(R& r, const V& v) const {
    if (sputsoft::numbers::is_negative(v))
      sputsoft::numbers::negate(r, v);
    else
      sputsoft::numbers::set(r, v);
  }
};

/* Binary */

template <typename Op>
struct resolve_binary<Op, unsigned, unsigned> {
  typedef unsigned return_type;
};

template <typename Op>
struct resolve_binary<Op, unsigned, signed> {
  typedef unsigned return_type;
};

template <typename Op>
struct resolve_binary<Op, signed, unsigned> {
  typedef unsigned return_type;
};

template <typename Op, typename R, typename V1, typename V2>
struct evaluator_rvv {
  void operator()(R& r, const V1& v1, const V2& v2) const {
    sputsoft::numbers::set(r, function_vv<Op, V1, V2>()(v1, v2));
  }
};

template <typename Op, typename V1, typename V2>
struct function_vv {
  typedef typename resolve_binary<Op, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return_type r;
    evaluator_rvv<Op, return_type, V1, V2>()(r, v1, v2);
    return r;
  }
};

/* Binary add */

template <typename V1, typename V2>
struct function_vv<ops::binary::add, V1, V2> {
  typedef typename resolve_binary<ops::binary::add, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return v1 + v2;
  }
};

/* Binary sub */

template <typename V1, typename V2>
struct function_vv<ops::binary::sub, V1, V2> {
  typedef typename resolve_binary<ops::binary::sub, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return v1 - v2;
  }
};

/* Binary mul */

template <typename V1, typename V2>
struct function_vv<ops::binary::mul, V1, V2> {
  typedef typename resolve_binary<ops::binary::mul, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return v1 * v2;
  }
};

/* Binary div */

template <typename V1, typename V2>
struct function_vv<ops::binary::div, V1, V2> {
  typedef typename resolve_binary<ops::binary::div, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return v1 / v2;
  }
};

/* Binary rem */

template <typename V1, typename V2>
struct function_vv<ops::binary::rem, V1, V2> {
  typedef typename resolve_binary<ops::binary::rem, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return v1 % v2;
  }
};

/* Binary bit_and */

template <typename V1, typename V2>
struct function_vv<ops::binary::bit_and, V1, V2> {
  typedef typename resolve_binary<ops::binary::bit_and, V1, V2>::return_type return_type;
  return_type operator()(const V1& v1, const V2& v2) const {
    return v1 & v2;
  }
};


template <typename Q, typename R, typename V1, typename V2>
struct quotrem_4_eval {
  static inline void quotrem(Q& q, R& r, const V1& v1, const V2& v2) {
    sputsoft::numbers::div(q, v1, v2);
    sputsoft::numbers::rem(r, v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct or_3_eval {
  static inline void bit_or(R& r, const V1& v1, const V2& v2) { r = v1 | v2; }
};

template <typename R, typename V1, typename V2>
struct xor_3_eval {
  static inline void bit_xor(R& r, const V1& v1, const V2& v2) { r = v1 ^ v2; }
};

template <typename R, typename V1, typename V2>
struct and_not_3_eval {
  static inline void bit_and_not(R& r, const V1& v1, const V2& v2) { r = v1 & ~v2; }
};

template <typename V1, typename V2>
struct cmp_r2_eval {
  static inline int cmp(const V1& v1, const V2& v2) {
    return v1 < v2 ? -1 : v1 > v2 ? 1 : 0;
  }
};

template <typename V1, typename V2>
struct equal_r2_eval {
  static inline bool equal(const V1& v1, const V2& v2) {
    return sputsoft::numbers::compare(v1, v2) == 0;
  }
};

template <typename T>
struct is_zero_r1_eval {
  static inline bool is_zero(const T& v) {
    return !v;
  }
};

template <typename T>
struct is_positive_r1_eval {
  static inline bool is_positive(const T& v) {
    return v > 0;
  }
};

template <typename T>
struct is_negative_r1_eval {
  static inline bool is_negative(const T& v) {
    return v < 0;
  }
};

template <typename T>
struct log2_floor_evaluator {
  static std::size_t log2_floor(T n) {
    std::size_t r = -1;
    while (n) {
      sputsoft::numbers::bit_shift_right(n, n, 1);
      ++r;
    }
    return r;
  }
};

template <typename R, typename V>
struct lshift_3_eval {
  static inline void lshift(R& r, const V& v, std::ptrdiff_t count) {
    r = count == 0 ? v : count > 0 ? v << count : v >> (-count);
  }
};

template <typename R, typename V>
struct rshift_3_eval {
  static inline void rshift(R& r, const V& v, std::ptrdiff_t count) {
    r = count == 0 ? v : count > 0 ? v >> count : v << (-count);
  }
};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
