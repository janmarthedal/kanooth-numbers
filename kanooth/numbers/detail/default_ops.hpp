/*
 * File:   kanooth/numbers/detail/default_ops.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-06-01 15:05Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_DEFAULT_OPS_HPP
#define	_KANOOTH_NUMBERS_DETAIL_DEFAULT_OPS_HPP

#include <kanooth/numbers/detail/named_ops.hpp>
#include <cmath>          // for sqrt, floor, ceil

namespace kanooth {
namespace numbers {

template <typename T>
struct eval_result : type_if<is_number<T>::value, T> {};

namespace detail {

template <typename Op, typename R, typename V1, typename V2>
struct function_rt_vv_default;

template <typename Op, typename R, typename V1, typename V2>
struct function_rt_vv
  : public function_rt_vv_default<Op, R, V1, V2> {};

template <typename Op, typename R, typename V>
struct enabler_rv
  : enabler_rv<ops::unary::identity, R, typename unary_result<Op, V>::type> {};

template <typename R, typename V>
struct enabler_rv<ops::unary::identity, R, V>
  : type_if<is_assignable<R, V>::value, void> {};

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

template <typename T>
struct unary_result2<ops::unary::identity, T> : public type_if<true, T> {};

template <typename R, typename V>
struct evaluator_rv<ops::unary::identity, R, V> {
  inline void operator()(R& r, const V& v) const {
    r = v;
  }
};

/* Unary eval/identity */

template <typename T>
struct function_v<ops::unary::identity, T> {
  inline const T& operator()(const T& v) const { return v; }
};

/* Unary negate */

template <typename T>
struct unary_result2<ops::unary::negate, T> : public make_signed<T> {};

template <typename V>
struct function_v<ops::unary::negate, V> {
  typedef typename unary_result<ops::unary::negate, V>::type return_type;
  inline return_type operator()(const V& v) const { return -((return_type) v); }
};

/* Unary abs */

template <typename T>
struct unary_result2<ops::unary::abs, T> : public make_unsigned<T> {};

template <typename V>
struct function_v<ops::unary::abs, V> {
  typedef typename unary_result<ops::unary::abs, V>::type return_type;
  inline return_type operator()(const V& v) const { return v < 0 ? -v : v; }
};

/* Unary bit_not */

template <typename T>
struct unary_result2<ops::unary::bit_not, T> : public unary_result<ops::unary::negate, T> {};

template <typename V>
struct function_v<ops::unary::bit_not, V> {
  typedef typename unary_result<ops::unary::bit_not, V>::type return_type;
  inline return_type operator()(const V& v) const { return ~v; }
};

/* Unary trunc */

template <typename T>
struct unary_result2<ops::unary::trunc, T>
  : public type_if<kanooth::is_native_number<T>::value, T> {};

namespace {
  template <typename T, bool IsNativeInt>
  struct _function_v_trunc {
    typedef typename unary_result<ops::unary::trunc, T>::type return_type;
    inline return_type operator()(const T& v) const {
      return v < 0 ? kanooth::numbers::ceil(v) : kanooth::numbers::floor(v);
    }
  };
  template <typename T>
  struct _function_v_trunc<T, true> {
    typedef typename unary_result<ops::unary::trunc, T>::type return_type;
    inline return_type operator()(const T& v) const { return (return_type) v; }
  };
}

template <typename V>
struct function_v<ops::unary::trunc, V>
  : _function_v_trunc<V, kanooth::is_native_int<V>::value> {};

/* Unary floor */

template <typename T>
struct unary_result2<ops::unary::floor, T> : public unary_result2<ops::unary::trunc, T> {};

namespace {
  template <typename T, bool IsNativeInt>
  struct _function_v_floor {
    typedef typename unary_result<ops::unary::floor, T>::type return_type;
    inline return_type operator()(const T& v) const { return (return_type) std::floor(v); }
  };
  template <typename T>
  struct _function_v_floor<T, true>
    : public function_v<ops::unary::trunc, T> {};
}

template <typename V>
struct function_v<ops::unary::floor, V>
  : _function_v_floor<V, kanooth::is_native_int<V>::value> {};

/* Unary ceil */

template <typename T>
struct unary_result2<ops::unary::ceil, T> : public unary_result2<ops::unary::trunc, T> {};

namespace {
  template <typename T, bool IsNativeInt>
  struct _function_v_ceil {
    typedef typename unary_result<ops::unary::ceil, T>::type return_type;
    inline return_type operator()(const T& v) const { return (return_type) std::ceil(v); }
  };
  template <typename T>
  struct _function_v_ceil<T, true>
    : public function_v<ops::unary::trunc, T> {};
}

template <typename V>
struct function_v<ops::unary::ceil, V>
  : _function_v_ceil<V, kanooth::is_native_int<V>::value> {};

/* Unary round */

template <typename T>
struct unary_result2<ops::unary::round, T> : public unary_result2<ops::unary::trunc, T> {};

namespace {
  template <typename T, bool IsNativeInt>
  struct _function_v_round {
    typedef typename unary_result<ops::unary::round, T>::type return_type;
    inline return_type operator()(const T& v) const {
      return kanooth::numbers::floor(v + 0.5);
    }
  };
  template <typename T>
  struct _function_v_round<T, true>
    : public function_v<ops::unary::trunc, T> {};
}

template <typename V>
struct function_v<ops::unary::round, V>
  : _function_v_round<V, kanooth::is_native_int<V>::value> {};

/* sqrt */

template <typename T>
struct unary_result2<ops::unary::sqrt, T>
        : public type_if<kanooth::is_native_float<T>::value, T> {};

template <typename V>
struct function_v<ops::unary::sqrt, V> {
  typedef typename unary_result<ops::unary::sqrt, V>::type return_type;
  inline return_type operator()(const V& v) const { return std::sqrt(v); }
};

/* Binary */

template <typename Op, typename R, typename V1, typename V2>
struct evaluator_rvv {
  void operator()(R& r, const V1& v1, const V2& v2) const {
    kanooth::numbers::set(r, function_vv<Op, V1, V2>()(v1, v2));
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
struct binary_result2<ops::binary::sub, V1, V2>
  : public choose_type<!kanooth::is_signed<V1>::value && !kanooth::is_signed<V2>::value,
                       V1, typename kanooth::numbers::common_type<V1, V2>::type> {};

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

/* Binary div */

namespace {
template <typename V1, typename V2, bool IsIntegral>
struct _binary_result_div
        : public kanooth::numbers::common_type<V1, V2> {};
template <typename V1, typename V2>
struct _binary_result_div<V1, V2, true>
        : public kanooth::make_signed_if<kanooth::is_signed<V2>::value, V1> {};
}

template <typename V1, typename V2>
struct binary_result2<ops::binary::div, V1, V2>
        : public _binary_result_div<V1, V2, kanooth::is_integral<V1>::value && kanooth::is_integral<V2>::value> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::div, R, V1, V2> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 / v2;
  }
};

/* Binary trunc div */

template <typename V1, typename V2>
struct binary_result2<ops::binary::trunc_div, V1, V2>
  : public unary_result<ops::unary::trunc,
                        typename binary_result<ops::binary::div, V1, V2>::type> {};

namespace {
template <typename R, typename V1, typename V2, bool IsNativeInt>
struct _function_rt_vv_default_trunc_div {
  inline R operator()(const V1& v1, const V2& v2) const {
    return kanooth::numbers::trunc(kanooth::numbers::div(v1, v2));
  }
};

template <typename R, typename V1, typename V2>
struct _function_rt_vv_default_trunc_div<R, V1, V2, true> {
  inline R operator()(const V1& v1, const V2& v2) const {
    return v1 / v2;
  }
};
}

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::trunc_div, R, V1, V2>
        : public _function_rt_vv_default_trunc_div<R, V1, V2,
                kanooth::is_native_int<V1>::value && kanooth::is_native_int<V2>::value> {};

/* Binary floor div */

template <typename V1, typename V2>
struct binary_result2<ops::binary::floor_div, V1, V2>
  : public binary_result<ops::binary::trunc_div, V1, V2> {};

namespace {
  template <typename R, typename V1, typename V2, bool IsNativeInt>
  struct _function_rt_vv_default_floor_div {
    inline R operator()(const V1& v1, const V2& v2) const {
      return kanooth::numbers::floor(kanooth::numbers::div(v1, v2));
    }
  };

  template <typename R, typename V1, typename V2>
  struct _function_rt_vv_default_floor_div<R, V1, V2, true> {
    inline R operator()(const V1& v1, const V2& v2) const {
      if (v1 >= 0) {
        if (v2 < 0)
          return (v1 - v2 - 1) / v2;
      } else if (v2 >= 0)
        return (v1 - v2 + 1) / v2;
      return v1 / v2;
    }
  };
}

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::floor_div, R, V1, V2>
        : public _function_rt_vv_default_floor_div<R, V1, V2,
                kanooth::is_native_int<V1>::value && kanooth::is_native_int<V2>::value> {};

/* Binary ceil div */

template <typename V1, typename V2>
struct binary_result2<ops::binary::ceil_div, V1, V2>
  : public binary_result<ops::binary::trunc_div, V1, V2> {};

namespace {
  template <typename R, typename V1, typename V2, bool IsNativeInt>
  struct _function_rt_vv_default_ceil_div {
    inline R operator()(const V1& v1, const V2& v2) const {
      return kanooth::numbers::ceil(kanooth::numbers::div(v1, v2));
    }
  };

  template <typename R, typename V1, typename V2>
  struct _function_rt_vv_default_ceil_div<R, V1, V2, true> {
    inline R operator()(const V1& v1, const V2& v2) const {
      if (v1 >= 0) {
        if (v2 >= 0)
          return (v1 + v2 - 1) / v2;
      } else if (v2 < 0)
        return (v1 + v2 + 1) / v2;
      return v1 / v2;
    }
  };
}

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::ceil_div, R, V1, V2>
        : public _function_rt_vv_default_ceil_div<R, V1, V2,
                kanooth::is_native_int<V1>::value && kanooth::is_native_int<V2>::value> {};

/* Binary rem */

template <typename V1, typename V2>
struct binary_result2<ops::binary::rem, V1, V2>
  : public binary_result2<ops::binary::trunc_rem, V1, V2> {};

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::rem, R, V1, V2>
        : public function_rt_vv_default<ops::binary::trunc_rem, R, V1, V2> {};

/* Binary trunc rem */

namespace {
template <typename V1, typename V2, bool IsIntegral>
struct _binary_result_trunc_rem
        : public kanooth::numbers::common_type<V1, V2> {};
template <typename V1, typename V2>
struct _binary_result_trunc_rem<V1, V2, true>
        : public kanooth::make_signed_if<kanooth::is_signed<V1>::value, V2> {};
}

template <typename V1, typename V2>
struct binary_result2<ops::binary::trunc_rem, V1, V2>
        : public _binary_result_trunc_rem<V1, V2, kanooth::is_integral<V1>::value && kanooth::is_integral<V2>::value> {};

namespace {
  template <typename R, typename V1, typename V2, bool IsNativeInt>
  struct _function_rt_vv_default_trunc_rem {
    inline R operator()(const V1& v1, const V2& v2) const {
      return kanooth::numbers::sub(v1, kanooth::numbers::mul(kanooth::numbers::trunc_div(v1, v2), v2));
    }
  };

  template <typename R, typename V1, typename V2>
  struct _function_rt_vv_default_trunc_rem<R, V1, V2, true> {
    inline R operator()(const V1& v1, const V2& v2) const {
      return v1 % v2;
    }
  };
}

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::trunc_rem, R, V1, V2>
        : public _function_rt_vv_default_trunc_rem<R, V1, V2,
                kanooth::is_native_int<V1>::value && kanooth::is_native_int<V2>::value> {};

/* Binary floor rem */

template <typename V1, typename V2>
struct binary_result2<ops::binary::floor_rem, V1, V2>
  : public binary_result<ops::binary::trunc_rem, V1, V2> {};

namespace {
  template <typename R, typename V1, typename V2, bool IsNativeInt>
  struct _function_rt_vv_default_floor_rem {
    inline R operator()(const V1& v1, const V2& v2) const {
      return kanooth::numbers::sub(v1, kanooth::numbers::mul(kanooth::numbers::floor_div(v1, v2), v2));
    }
  };

  template <typename R, typename V1, typename V2>
  struct _function_rt_vv_default_floor_rem<R, V1, V2, true> {
    inline R operator()(const V1& v1, const V2& v2) const {
      R r = v1 % v2;
      if (r && ((v1 >= 0) != (v2 >= 0)))
        r += v2;
      return r;
    }
  };
}

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::floor_rem, R, V1, V2>
        : public _function_rt_vv_default_floor_rem<R, V1, V2,
                kanooth::is_native_int<V1>::value && kanooth::is_native_int<V2>::value> {};

/* Binary ceil rem */

template <typename V1, typename V2>
struct binary_result2<ops::binary::ceil_rem, V1, V2>
  : public binary_result<ops::binary::trunc_rem, V1, V2> {};

namespace {
  template <typename R, typename V1, typename V2, bool IsNativeInt>
  struct _function_rt_vv_default_ceil_rem {
    inline R operator()(const V1& v1, const V2& v2) const {
      return kanooth::numbers::sub(v1, kanooth::numbers::mul(kanooth::numbers::ceil_div(v1, v2), v2));
    }
  };

  template <typename R, typename V1, typename V2>
  struct _function_rt_vv_default_ceil_rem<R, V1, V2, true> {
    inline R operator()(const V1& v1, const V2& v2) const {
      R r = v1 % v2;
      if (r && ((v1 >= 0) == (v2 >= 0)))
        r -= v2;
      return r;
    }
  };
}

template <typename R, typename V1, typename V2>
struct function_rt_vv_default<ops::binary::ceil_rem, R, V1, V2>
        : public _function_rt_vv_default_ceil_rem<R, V1, V2,
                kanooth::is_native_int<V1>::value && kanooth::is_native_int<V2>::value> {};

/* Quotient and remainder */

template <typename Q, typename R, typename V1, typename V2>
struct function_divrem {
  R operator()(Q& q, const V1& v1, const V2& v2) const {
    R r = kanooth::numbers::rem(v1, v2);
    kanooth::numbers::div(q, v1, v2);    // may change v1 if q==v1
    return r;
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct function_floor_divrem {
  R operator()(Q& q, const V1& v1, const V2& v2) const {
    R r = kanooth::numbers::floor_rem(v1, v2);
    kanooth::numbers::floor_div(q, v1, v2);    // may change v1 if q==v1
    return r;
  }
};

template <typename Q, typename R, typename V1, typename V2>
struct divrem_evaluator {
  void operator()(Q& q, R& r, const V1& v1, const V2& v2) const {
    kanooth::numbers::div(q, v1, v2);
    kanooth::numbers::rem(r, v1, v2);
  }
};

/* Binary bitwise ops */

template <typename V1, typename V2>
struct binary_result2<ops::binary::bit_and, V1, V2>
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

template <typename V1, typename V2>
struct binary_result2<ops::binary::shift_left, V1, V2>
  : public type_if<is_integral<V1>::value && is_native_int<V2>::value, V1> {};

template <typename R, typename V>
struct function_rt_vv_default<ops::binary::shift_left, R, V, std::ptrdiff_t> {
  inline R operator()(const V& v, std::ptrdiff_t count) const {
    return v << count;
  }
};

template <typename V1, typename V2>
struct binary_result2<ops::binary::shift_right, V1, V2>
  : public binary_result<ops::binary::shift_left, V1, V2> {};

template <typename R, typename V>
struct function_rt_vv_default<ops::binary::shift_right, R, V, std::ptrdiff_t> {
  inline R operator()(const V& v, std::ptrdiff_t count) const {
    return v >> count;
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
struct bool_compare_eval_default<ops::binary_compare::not_equal, V1, V2> {
  inline bool operator()(const V1& v1, const V2& v2) const {
    return v1 != v2;
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
    return v == 0;
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

namespace {

short floor_log2_table[256] = {
 -1, 0, 1,1, 2,2,2,2, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 };

template <typename T, unsigned bits>
struct floor_log2_eval3;

template <typename T>
struct floor_log2_eval3<T, 8> {
  std::size_t operator()(const T n) const {
    return floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval3<T, 16> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (n & (ones << 8)) { n >>= 8; r += 8; }
    return r + floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval3<T, 32> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (n & (ones << 16)) { n >>= 16; r += 16; }
    if (n & (ones <<  8)) { n >>=  8; r +=  8; }
    return r + floor_log2_table[n];
  }
};

template <typename T>
struct floor_log2_eval3<T, 64> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (n & (ones << 32)) { n >>= 32; r += 32; }
    if (n & (ones << 16)) { n >>= 16; r += 16; }
    if (n & (ones <<  8)) { n >>=  8; r +=  8; }
    return r + floor_log2_table[n];
  }
};

template <typename T, bool Ok>
struct floor_log2_eval2;

template <typename T>
struct floor_log2_eval2<T, true> {
  inline std::size_t operator()(const T n) const {
    typedef typename make_unsigned<T>::type S;
    return floor_log2_eval3<S, kanooth::number_bits<S>::value>()((S) n);
  }
};

}

template <typename T>
struct floor_log2_eval
  : public floor_log2_eval2<T, kanooth::is_native_int<T>::value> {};

/**************** right-most bit ****************/

namespace {

short ruler_table[256] = {
 -1, 0, 1,0, 2,0,1,0, 3,0,1,0,2,0,1,0, 4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
 5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0 };

template <typename T, unsigned bits>
struct ruler_eval3;

template <typename T>
struct ruler_eval3<T, 8> {
  std::size_t operator()(const T n) const {
    return ruler_table[n & 0xff];
  }
};

template <typename T>
struct ruler_eval3<T, 16> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (!(n & ~(ones << 8))) { n >>= 8; r += 8; }
    return r + ruler_table[n & 0xff];
  }
};

template <typename T>
struct ruler_eval3<T, 32> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (!(n & ~(ones << 16))) { n >>= 16; r += 16; }
    if (!(n & ~(ones <<  8))) { n >>=  8; r +=  8; }
    return r + ruler_table[n & 0xff];
  }
};

template <typename T>
struct ruler_eval3<T, 64> {
  std::size_t operator()(T n) const {
    static const T ones = -1;
    std::size_t r = 0;
    if (!(n & ~(ones << 32))) { n >>= 32; r += 32; }
    if (!(n & ~(ones << 16))) { n >>= 16; r += 16; }
    if (!(n & ~(ones <<  8))) { n >>=  8; r +=  8; }
    return r + ruler_table[n & 0xff];
  }
};

template <typename T, bool Ok>
struct ruler_eval2;

template <typename T>
struct ruler_eval2<T, true> {
  inline std::size_t operator()(const T& n) const {
    typedef typename make_unsigned<T>::type S;
    return ruler_eval3<S, kanooth::number_bits<S>::value>()((S) n);
  }
};

}

template <typename T>
struct ruler_eval
  : public ruler_eval2<T, kanooth::is_native_int<T>::value> {};

/**************** show_binary ****************/

namespace {

const char* toBinSt[16] = {
  "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
  "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

template <typename T, unsigned bits>
struct show_binary_eval3;

template <typename T>
struct show_binary_eval3<T, 8> {
  std::ostream& operator()(std::ostream& os, const T n) const {
    os << toBinSt[n >> 4];
    os << toBinSt[n & 0xf];
    return os;
  }
};

template <typename T>
struct show_binary_eval3<T, 16> {
  std::ostream& operator()(std::ostream& os, const T n) const {
    show_binary_eval3<T, 8>()(os, n >> 8);
    show_binary_eval3<T, 8>()(os, n & 0xff);
    return os;
  }
};

template <typename T>
struct show_binary_eval3<T, 32> {
  std::ostream& operator()(std::ostream& os, const T n) const {
    show_binary_eval3<T, 16>()(os, n >> 16);
    show_binary_eval3<T, 16>()(os, n & 0xffffu);
    return os;
  }
};

template <typename T>
struct show_binary_eval3<T, 64> {
  std::ostream& operator()(std::ostream& os, const T n) const {
    show_binary_eval3<T, 32>()(os, n >> 32);
    show_binary_eval3<T, 32>()(os, n & 0xffffffffu);
    return os;
  }
};

template <typename T, bool Ok>
struct show_binary_eval2;

template <typename T>
struct show_binary_eval2<T, true> {
  std::ostream& operator()(std::ostream& os, const T n) const {
    typedef typename kanooth::make_unsigned<T>::type S;
    return show_binary_eval3<S, kanooth::number_bits<S>::value>()(os, (S) n);
  }
};

}

template <typename T>
struct show_binary_eval
  : public show_binary_eval2<T, kanooth::is_native_int<T>::value> {};

/**************** test_bit ****************/

namespace {

  template <typename T, bool Ok>
  struct test_bit_eval2;

  template <typename T>
  struct test_bit_eval2<T, true> {
    inline bool operator()(const T n, std::size_t pos) const {
      return n & (T(1) << pos);
    }
  };

}

template <typename T>
struct test_bit_eval
  : public test_bit_eval2<T, kanooth::is_native_int<T>::value> {};

/********************************************/

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_DEFAULT_OPS_HPP
