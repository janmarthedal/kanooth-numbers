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
}

template <typename N>
struct is_number<detail::numb<N> > : public true_type {};

namespace detail {

template <typename N>
struct enabler_rv<ops::unary::identity, numb<N>, std::string> : public enable_if<true, void> {};

template <typename N>
struct enabler_rv<ops::unary::identity, numb<N>, const char*> : public enable_if<true, void> {};

// evaluator_rv

template <typename T, typename V>
struct evaluator_rv<ops::unary::identity, numb<T>, V> {
  inline void operator()(numb<T>& r, const V& v) const {
    r.set(v);
  }
};

// function_rt_vv

namespace {

template <typename Op, typename R, typename V1, typename V2>
struct to_numb_function_rt_vv {
  R operator()(const V1& v1, const V2& v2) const {
    R r;
    evaluator_rvv<Op, R, V1, V2>()(r, v1, v2);
    return r;
  }
};

template <typename Op, typename R, typename V1, typename V2, typename EVAL>
struct simple_eval_vv;

template <typename R, typename V1, typename V2, typename EVAL>
struct simple_eval_vv<ops::binary::rem, R, V1, V2, EVAL> {
  R operator()(const V1& v1, const V2& v2) const {
    return EVAL::rem(v1, v2);
  }
};

template <typename R, typename V1, typename V2, typename EVAL>
struct simple_eval_vv<ops::binary::sub, R, V1, V2, EVAL> {
  R operator()(const V1& v1, const V2& v2) const {
    return EVAL::sub(v1, v2);
  }
};

template <typename Op, typename R, typename V1, typename V2>
struct to_simple_function_rt_vv
  : simple_eval_vv<Op, R, V1, V2,
           typename choose_type<type_rank<V1>::value >= type_rank<V2>::value, V1, V2>::type> {};

}

template <typename Op, typename N1, typename N2, typename N3>
struct function_rt_vv<Op, numb<N1>, numb<N2>, numb<N3> >
  : to_numb_function_rt_vv<Op, numb<N1>, numb<N2>, numb<N3> > {};

template <typename Op, typename N1, typename N2, typename V>
struct function_rt_vv<Op, numb<N1>, numb<N2>, V>
  : to_numb_function_rt_vv<Op, numb<N1>, numb<N2>, V> {};

template <typename Op, typename N1, typename V, typename N2>
struct function_rt_vv<Op, numb<N1>, V, numb<N2> >
  : to_numb_function_rt_vv<Op, numb<N1>, V, numb<N2> > {};

template <typename Op, typename R, typename N1, typename N2>
struct function_rt_vv<Op, R, numb<N1>, numb<N2> >
  : to_simple_function_rt_vv<Op, R, numb<N1>, numb<N2> > {};

template <typename Op, typename R, typename V, typename N>
struct function_rt_vv<Op, R, V, numb<N> >
  : to_simple_function_rt_vv<Op, R, V, numb<N> > {};

template <typename Op, typename R, typename N, typename V>
struct function_rt_vv<Op, R, numb<N>, V>
  : to_simple_function_rt_vv<Op, R, numb<N>, V> {};

// evaluator_rvv

namespace {

template <typename Op, typename R, typename V1, typename V2>
struct numb_assign_vv;

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::add, N, V1, V2> {
  void operator()(N& r, const V1& v1, const V2& v2) const {
    r.add(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::sub, N, V1, V2> {
  void operator()(N& r, const V1& v1, const V2& v2) const {
    r.sub(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::mul, N, V1, V2> {
  void operator()(N& r, const V1& v1, const V2& v2) const {
    r.mul(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::div, N, V1, V2> {
  void operator()(N& r, const V1& v1, const V2& v2) const {
    r.div(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::rem, N, V1, V2> {
  void operator()(N& r, const V1& v1, const V2& v2) const {
    r.rem(v1, v2);
  }
};

template <typename Op, typename N, typename R, typename V1, typename V2>
struct numb_evaluator_rvv {
  void operator()(N& r, const V1& v1, const V2& v2) const {
    sputsoft::numbers::set(r, function_vv<Op, V1, V2>()(v1, v2));
  }
};

template <typename Op, typename N1, typename N2, typename V1, typename V2>
struct numb_evaluator_rvv<Op, numb<N1>, numb<N2>, V1, V2>
  : public numb_assign_vv<Op, numb<N1>, V1, V2> {};

}

template <typename Op, typename N, typename V1, typename V2>
struct evaluator_rvv<Op, numb<N>, V1, V2>
  : public numb_evaluator_rvv<Op, numb<N>, typename binary_result<Op, V1, V2>::type, V1, V2> {};


template <typename N, typename R, typename V1, typename V2>
struct function_divrem<numb<N>, R, V1, V2> {
  R operator()(numb<N>& q, const V1& v1, const V2& v2) const {
    return q.divrem(v1, v2);
  }
};

/*
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
};*/

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NUMBER_ABST_HPP
