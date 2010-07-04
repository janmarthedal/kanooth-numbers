/* 
 * File:   sputsoft/numbers/detail/number_abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-19 15:10Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NUMBER_ABST_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_NUMBER_ABST_HPP

#include <sputsoft/detail/types.hpp>
#include <sputsoft/numbers/detail/named_ops.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename N> class numb;

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, unsigned short> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, unsigned short, numb<T> > {
  typedef numb<T> return_type;
};

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, short> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, short, numb<T> > {
  typedef numb<T> return_type;
};

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, unsigned> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, unsigned, numb<T> > {
  typedef numb<T> return_type;
};

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, int> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, int, numb<T> > {
  typedef numb<T> return_type;
};

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, unsigned long> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, unsigned long, numb<T> > {
  typedef numb<T> return_type;
};

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, long> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, long, numb<T> > {
  typedef numb<T> return_type;
};

#ifdef SPUTSOFT_HAS_LONG_LONG
template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, long long> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, long long, numb<T> > {
  typedef numb<T> return_type;
};

template <typename Op, typename T>
struct resolve_binary<Op, numb<T>, unsigned long long> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, unsigned long long, numb<T> > {
  typedef numb<T> return_type;
};
#endif

template <typename T, typename V>
struct set_2_eval<numb<T>, V> {
  static void set(numb<T>& r, const V& v) {
    r.set(v);
  }
};

template <typename T, typename V>
struct negate_2_eval<numb<T>, V> {
  static void negate(numb<T>& r, const V& v) {
    r.negate(v);
  }
};

template <typename T, typename V1, typename V2>
struct add_3_eval<numb<T>, V1, V2> {
  static void add(numb<T>& r, const V1& v1, const V2& v2) {
    r.add(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct sub_3_eval<numb<T>, V1, V2> {
  static void sub(numb<T>& r, const V1& v1, const V2& v2) {
    r.sub(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct mul_3_eval<numb<T>, V1, V2> {
  static void mul(numb<T>& r, const V1& v1, const V2& v2) {
    r.mul(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct div_3_eval<numb<T>, V1, V2> {
  static void div(numb<T>& r, const V1& v1, const V2& v2) {
    r.div(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct div_floor_3_eval<numb<T>, V1, V2> {
  static void div_floor(numb<T>& r, const V1& v1, const V2& v2) {
    r.div_floor(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct div_ceil_3_eval<numb<T>, V1, V2> {
  static void div_ceil(numb<T>& r, const V1& v1, const V2& v2) {
    r.div_ceil(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct div_trunc_3_eval<numb<T>, V1, V2> {
  static void div_trunc(numb<T>& r, const V1& v1, const V2& v2) {
    r.div_trunc(v1, v2);
  }
};

template <typename R, typename T1, typename V2>
struct rem_3_eval<R, numb<T1>, V2> {
  static void rem(R& r, const numb<T1>& v1, const V2& v2) {
    numb<T1>::rem(r, v1, v2);
  }
};

template <typename R, typename T1, typename V2>
struct rem_floor_3_eval<R, numb<T1>, V2> {
  static void rem_floor(R& r, const numb<T1>& v1, const V2& v2) {
    numb<T1>::rem_floor(r, v1, v2);
  }
};

template <typename R, typename T1, typename V2>
struct rem_ceil_3_eval<R, numb<T1>, V2> {
  static void rem_ceil(R& r, const numb<T1>& v1, const V2& v2) {
    numb<T1>::rem_ceil(r, v1, v2);
  }
};

template <typename R, typename T1, typename V2>
struct rem_trunc_3_eval<R, numb<T1>, V2> {
  static void rem_trunc(R& r, const numb<T1>& v1, const V2& v2) {
    numb<T1>::rem_trunc(r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct quotrem_4_eval<numb<T>, R, V1, V2> {
  static void quotrem(numb<T>& q, R& r, const V1& v1, const V2& v2) {
    numb<T>::quotrem(q, r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct quotrem_floor_4_eval<numb<T>, R, V1, V2> {
  static void quotrem_floor(numb<T>& q, R& r, const V1& v1, const V2& v2) {
    numb<T>::quotrem_floor(q, r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct quotrem_ceil_4_eval<numb<T>, R, V1, V2> {
  static void quotrem_ceil(numb<T>& q, R& r, const V1& v1, const V2& v2) {
    numb<T>::quotrem_ceil(q, r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct quotrem_trunc_4_eval<numb<T>, R, V1, V2> {
  static void quotrem_trunc(numb<T>& q, R& r, const V1& v1, const V2& v2) {
    numb<T>::quotrem_trunc(q, r, v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct and_3_eval<numb<T>, V1, V2> {
  static void bit_and(numb<T>& r, const V1& v1, const V2& v2) {
    r.bitwise_and(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct or_3_eval<numb<T>, V1, V2> {
  static void bit_or(numb<T>& r, const V1& v1, const V2& v2) {
    r.bitwise_or(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct xor_3_eval<numb<T>, V1, V2> {
  static void bit_xor(numb<T>& r, const V1& v1, const V2& v2) {
    r.bitwise_xor(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct and_not_3_eval<numb<T>, V1, V2> {
  static void bit_and_not(numb<T>& r, const V1& v1, const V2& v2) {
    r.bitwise_and_not(v1, v2);
  }
};

template <typename T1, typename T2>
struct cmp_r2_eval<numb<T1>, numb<T2> > {
  inline static int cmp(const numb<T1>& v1, const numb<T2>& v2) {
    return v1.cmp(v2);
  }
};

template <typename T1, typename T2>
struct cmp_r2_eval<numb<T1>, T2 > {
  inline static int cmp(const numb<T1>& v1, const T2& v2) {
    return v1.cmp(v2);
  }
};

template <typename T1, typename T2>
struct cmp_r2_eval<T1, numb<T2> > {
  inline static int cmp(const T1& v1, const numb<T2>& v2) {
    return -v2.cmp(v1);
  }
};

template <typename T>
struct is_zero_r1_eval<numb<T> > {
  static inline bool is_zero(const numb<T>& v) {
    return v.is_zero();
  }
};

template <typename T, typename V>
struct lshift_3_eval<numb<T>, V> {
  static inline void lshift(numb<T>& r, const V& v, std::ptrdiff_t count) {
    r.left_shift(v, count);
  }
};

template <typename T, typename V>
struct rshift_3_eval<numb<T>, V> {
  static inline void rshift(numb<T>& r, const V& v, std::ptrdiff_t count) {
    r.right_shift(v, count);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_NUMBER_ABST_HPP
