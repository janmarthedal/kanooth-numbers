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

#include "named_ops.hpp"

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Op, typename T>
struct resolve_binary<Op, T, T> {
  typedef T return_type;
};

template <typename R, typename V>
struct set_2_eval {
  static inline void set(R& r, const V& v) { r = v; }
};

template <typename R, typename V>
struct negate_2_eval {
  static inline void negate(R& r, const V& v) { r = -v; }
};

template <typename R, typename V>
struct abs_2_eval {
  static inline void abs(R& r, const V& v) {
    if (sputsoft::numbers::is_negative(v))
      sputsoft::numbers::negate(r, v);
    else
      sputsoft::numbers::set(r, v);
  }
};

template <typename R, typename V1, typename V2>
struct add_3_eval {
  static inline void add(R& r, const V1& v1, const V2& v2) { r = v1 + v2; }
};

template <typename R, typename V1, typename V2>
struct sub_3_eval {
  static inline void sub(R& r, const V1& v1, const V2& v2) { r = v1 - v2; }
};

template <typename R, typename V1, typename V2>
struct mul_3_eval {
  static inline void mul(R& r, const V1& v1, const V2& v2) { r = v1 * v2; }
};

template <typename R, typename V1, typename V2>
struct div_3_eval {
  static inline void div(R& r, const V1& v1, const V2& v2) { r = v1 / v2; }
};

template <typename R, typename V1, typename V2>
struct rem_3_eval {
  static inline void rem(R& r, const V1& v1, const V2& v2) { r = v1 % v2; }
};

template <typename Q, typename R, typename V1, typename V2>
struct quotrem_4_eval {
  static inline void quotrem(Q& q, R& r, const V1& v1, const V2& v2) {
    sputsoft::numbers::div(q, v1, v2);
    sputsoft::numbers::rem(r, v1, v2);
  }
};

template <typename R, typename V1, typename V2>
struct and_3_eval {
  static inline void bit_and(R& r, const V1& v1, const V2& v2) { r = v1 & v2; }
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
