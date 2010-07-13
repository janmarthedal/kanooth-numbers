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

#include <sputsoft/type_traits.hpp>
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
struct evaluator_rv<ops::unary::identity, numb<T>, V> {
  void operator()(numb<T>& r, const V& v) const {
    r.set(v);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::add, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.add(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::sub, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.sub(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::mul, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.mul(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::div, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.div(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::div_floor, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.div_floor(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::div_ceil, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.div_ceil(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::div_trunc, numb<T>, V1, V2> {
  void operator()(numb<T>& r, const V1& v1, const V2& v2) const {
    r.div_trunc(v1, v2);
  }
};

template <typename R, typename T, typename V>
struct evaluator_rvv<ops::binary::rem, R, numb<T>, V> {
  void operator()(R& r, const numb<T>& v1, const V& v2) const {
    numb<T>::rem(r, v1, v2);
  }
};

template <typename R, typename T, typename V>
struct evaluator_rvv<ops::binary::rem_floor, R, numb<T>, V> {
  void operator()(R& r, const numb<T>& v1, const V& v2) const {
    numb<T>::rem_floor(r, v1, v2);
  }
};

template <typename R, typename T, typename V>
struct evaluator_rvv<ops::binary::rem_ceil, R, numb<T>, V> {
  void operator()(R& r, const numb<T>& v1, const V& v2) const {
    numb<T>::rem_ceil(r, v1, v2);
  }
};

template <typename R, typename T, typename V>
struct evaluator_rvv<ops::binary::rem_trunc, R, numb<T>, V> {
  void operator()(R& r, const numb<T>& v1, const V& v2) const {
    numb<T>::rem_trunc(r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct evaluator_rrvv<ops::binary::quotrem_floor, numb<T>, R, V1, V2> {
  void operator()(numb<T>& q, R& r, const V1& v1, const V2& v2) const {
    numb<T>::quotrem_floor(q, r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct evaluator_rrvv<ops::binary::quotrem_ceil, numb<T>, R, V1, V2> {
  void operator()(numb<T>& q, R& r, const V1& v1, const V2& v2) const {
    numb<T>::quotrem_ceil(q, r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct evaluator_rrvv<ops::binary::quotrem_trunc, numb<T>, R, V1, V2> {
  void operator()(numb<T>& q, R& r, const V1& v1, const V2& v2) const {
    numb<T>::quotrem_trunc(q, r, v1, v2);
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
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NUMBER_ABST_HPP
