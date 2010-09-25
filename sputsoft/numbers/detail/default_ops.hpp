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

namespace {

template <bool C, typename R, typename V>
struct identity_enabler {};

template <typename R, typename V>
struct identity_enabler<true, R, V>
  : public type_if<(detail::type_rank<R>::value >= detail::type_rank<V>::value)
                      && (is_signed<R>::value || !is_signed<V>::value), void> {};

}

template <typename Op, typename R, typename V1, typename V2>
struct function_rt_vv_default;

template <typename Op, typename R, typename V1, typename V2>
struct function_rt_vv : public function_rt_vv_default<Op, R, V1, V2> {};

template <typename Op, typename R, typename V>
struct enabler_rv
  : enabler_rv<ops::unary::identity, R, typename unary_result<Op, V>::type> {};

template <typename R, typename V>
struct enabler_rv<ops::unary::identity, R, V>
  : identity_enabler<is_number<R>::value && is_number<V>::value, R, V> {};

template <typename Op, typename R, typename V1, typename V2>
struct enabler_rvv
  : public enabler_rv<ops::unary::identity, R, typename binary_result<Op, V1, V2>::type> {};

/* Unary */

template <typename Op, typename R, typename V>
struct evaluator_rv {
  inline void operator()(R& r, const V& v) const {
    set(r, function_v<Op, V>()(v));
  }
};

/* Unary identity */

template <typename R, typename V>
struct evaluator_rv<ops::unary::identity, R, V> {
  inline void operator()(R& r, const V& v) const {
    r = v;
  }
};

/* Unary eval/identity */

/*template <typename T>
struct function_v<ops::unary::identity, T> {
  inline const T& operator()(const T& v) const { return v; }
};*/

/* Unary negate */

template <typename T>
struct unary_result<ops::unary::negate, T> : public make_signed<T> {};

template <typename V>
struct function_v<ops::unary::negate, V> {
  typedef typename unary_result<ops::unary::negate, V>::type return_type;
  inline return_type operator()(const V& v) const { return -((return_type) v); }
};

/* Unary abs */

template <typename T>
struct unary_result<ops::unary::abs, T> : public make_unsigned<T> {};

template <typename V>
struct function_v<ops::unary::abs, V> {
  typedef typename unary_result<ops::unary::abs, V>::type return_type;
  inline return_type operator()(const V& v) const { return v < 0 ? -v : v; }
};

/* Unary bit_not */

template <typename T>
struct unary_result<ops::unary::bit_not, T> : public unary_result<ops::unary::negate, T> {};

template <typename V>
struct function_v<ops::unary::bit_not, V> {
  typedef typename unary_result<ops::unary::bit_not, V>::type return_type;
  inline return_type operator()(const V& v) const { return ~v; }
};

template <typename T>
struct unary_result<ops::unary::trunc, T> : public type_if<sputsoft::is_integral<T>::value, T> {};

template <typename T>
struct unary_result<ops::unary::floor, T> : public unary_result<ops::unary::trunc, T> {};

template <typename T>
struct unary_result<ops::unary::ceil, T> : public unary_result<ops::unary::trunc, T> {};

/* Binary */

template <typename Op, typename R, typename V1, typename V2>
struct evaluator_rvv {
  void operator()(R& r, const V1& v1, const V2& v2) const {
    sputsoft::numbers::set(r, function_vv<Op, V1, V2>()(v1, v2));
  }
};

/* Binary add */

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::add, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 + v2;
  }
};

/* Binary sub */

template <typename V1, typename V2>
struct binary_result<ops::binary::sub, V1, V2>
  : public choose_type<!sputsoft::is_signed<V1>::value && !sputsoft::is_signed<V2>::value,
                       V1, typename sputsoft::numbers::common_type<V1, V2>::type> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::sub, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 - v2;
  }
};

/* Binary mul */

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::mul, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 * v2;
  }
};

/* Binary (floor, ceil, trunc) div */

template <typename V1, typename V2>
struct binary_result<ops::binary::div, V1, V2>
  : public choose_type<sputsoft::is_integral<V1>::value && sputsoft::is_integral<V2>::value,
        typename sputsoft::make_signed_if<sputsoft::is_signed<V2>::value, V1>::type,
        typename sputsoft::numbers::common_type<V1, V2>::type> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::div, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 / v2;
  }
};

template <typename V1, typename V2>
struct binary_result<ops::binary::trunc_div, V1, V2>
  : public unary_result<ops::unary::trunc,
                        typename binary_result<ops::binary::div, V1, V2>::type> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::trunc_div, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return sputsoft::numbers::trunc(sputsoft::numbers::div(v1, v2));
  }
};

template <typename V1, typename V2>
struct binary_result<ops::binary::floor_div, V1, V2>
  : public binary_result<ops::binary::trunc_div, V1, V2> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::floor_div, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return sputsoft::numbers::floor(sputsoft::numbers::div(v1, v2));
  }
};

template <typename V1, typename V2>
struct binary_result<ops::binary::ceil_div, V1, V2>
  : public binary_result<ops::binary::trunc_div, V1, V2> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::ceil_div, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return sputsoft::numbers::ceil(sputsoft::numbers::div(v1, v2));
  }
};

/* Binary rem */

template <typename V1, typename V2>
struct binary_result<ops::binary::rem, V1, V2>
  : public choose_type<sputsoft::is_integral<V1>::value && sputsoft::is_integral<V2>::value,
        typename sputsoft::make_signed_if<sputsoft::is_signed<V1>::value, V2>::type,
        typename sputsoft::numbers::common_type<V1, V2>::type> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::rem, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 % v2;
  }
};

/* Quotient and remainder */

template <typename Q, typename R, typename V1, typename V2>
struct function_divrem {
  R operator()(Q& q, const V1& v1, const V2& v2) const {
    R r = sputsoft::numbers::rem(v1, v2);
    sputsoft::numbers::div(q, v1, v2);    // may change v1 if q==v1
    return r;
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct function_floor_divrem {
  R operator()(Q& q, const V1& v1, const V2& v2) const {
    R r = sputsoft::numbers::floor_rem(v1, v2);
    sputsoft::numbers::floor_div(q, v1, v2);    // may change v1 if q==v1
    return r;
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct divrem_evaluator {
  void operator()(Q& q, R& r, const V1& v1, const V2& v2) const {
    sputsoft::numbers::div(q, v1, v2);
    sputsoft::numbers::rem(r, v1, v2);
  }
};

/* Binary bitwise ops */

template <typename V1, typename V2>
struct binary_result<ops::binary::bit_and, V1, V2>
  : public type_if<is_integral<V1>::value && is_integral<V2>::value,
             typename make_unsigned_if<!is_signed<V1>::value || !is_signed<V1>::value,
               typename choose_type<type_rank<V1>::value <= type_rank<V2>::value, V1, V2>::type
                                      >::type> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::bit_and, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 & v2;
  }
};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::bit_or, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 | v2;
  }
};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::bit_xor, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 ^ v2;
  }
};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::bit_and_not, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 & ~v2;
  }
};

// Bit shifting

template <typename T>
struct binary_result<ops::binary::shift_left, T, std::ptrdiff_t>
  : public type_if<is_integral<T>::value, T> {};

template <typename R, typename V>
struct function_rt_vv_default<ops::binary::shift_left, R, V, std::ptrdiff_t> {
  inline R operator()(const V& v, std::ptrdiff_t count) const {
    return (count == 0) ? v : (count > 0) ? (v << count) : (v >> (-count));
  }
};

// Comparisons

template <typename V1, typename V2>
struct compare_eval {
  inline int operator()(const V1& v1, const V2& v2) const {
    return v1 < v2 ? -1 : v1 > v2 ? 1 : 0;
  }
};

namespace {

template <typename Op, typename V1, typename V2>
struct bool_compare_eval_default;

template <typename V1, typename V2>
struct bool_compare_eval_default<ops::binary_compare::equal, V1, V2> {
  inline bool operator()(const V1& v1, const V2& v2) const {
    return v1 == v2;
  }
};

template <typename V1, typename V2>
struct bool_compare_eval_default<ops::binary_compare::less, V1, V2> {
  inline bool operator()(const V1& v1, const V2& v2) const {
    return v1 < v2;
  }
};

template <typename V1, typename V2>
struct bool_compare_eval_default<ops::binary_compare::greater, V1, V2> {
  inline bool operator()(const V1& v1, const V2& v2) const {
    return v1 > v2;
  }
};

template <typename V1, typename V2>
struct bool_compare_eval_default<ops::binary_compare::less_or_equal, V1, V2> {
  inline bool operator()(const V1& v1, const V2& v2) const {
    return v1 <= v2;
  }
};

template <typename V1, typename V2>
struct bool_compare_eval_default<ops::binary_compare::greater_or_equal, V1, V2> {
  inline bool operator()(const V1& v1, const V2& v2) const {
    return v1 >= v2;
  }
};

}

template <typename Op, typename V1, typename V2>
struct bool_compare_eval : public bool_compare_eval_default<Op, V1, V2> {};

template <typename T>
struct is_zero_eval {
  inline bool operator()(const T& v) const {
    return v != 0;
  }
};

template <typename T>
struct is_positive_eval {
  inline bool operator()(const T& v) const {
    return v > 0;
  }
};

template <typename T>
struct is_negative_eval {
  inline bool operator()(const T& v) const {
    return v < 0;
  }
};

/**************** floor log2 ****************/

short floor_log2_table[256] = {
 -1, 0, 1,1, 2,2,2,2, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 };

template <typename T, bool ok, unsigned bits>
struct floor_log2_eval2;

template <typename T>
struct floor_log2_eval2<T, true, 8> {
  std::size_t operator()(const T n) const {
    return floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval2<T, true, 16> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (n & (ones << 8)) { n >>= 8; r += 8; }
    return r + floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval2<T, true, 32> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (n & (ones << 16)) { n >>= 16; r += 16; }
    if (n & (ones <<  8)) { n >>=  8; r +=  8; }
    return r + floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval2<T, true, 64> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (n & (ones << 32)) { n >>= 32; r += 32; }
    if (n & (ones << 16)) { n >>= 16; r += 16; }
    if (n & (ones <<  8)) { n >>=  8; r +=  8; }
    return r + floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval
  : floor_log2_eval2<T, sputsoft::is_native_int<T>::value && !sputsoft::is_signed<T>::value,
                     sputsoft::number_bits<T>::value> {};

/********************************************/

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
