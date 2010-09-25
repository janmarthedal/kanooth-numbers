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

template <typename R, typename V1, typename V2, typename EVAL>
struct simple_eval_vv<ops::binary::bit_and, R, V1, V2, EVAL> {
  R operator()(const V1& v1, const V2& v2) const {
    return EVAL::bitwise_and(v1, v2);
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
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.add(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::sub, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.sub(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::mul, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.mul(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::div, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.div(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::rem, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.rem(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::bit_and, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.bitwise_and(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::bit_or, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.bitwise_or(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::bit_xor, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.bitwise_xor(v1, v2);
  }
};

template <typename N, typename V1, typename V2>
struct numb_assign_vv<ops::binary::bit_and_not, N, V1, V2> {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
    r.bitwise_and_not(v1, v2);
  }
};

template <typename N, typename V>
struct numb_assign_vv<ops::binary::shift_left, N, V, std::ptrdiff_t> {
  inline void operator()(N& r, const V& v, std::ptrdiff_t count) const {
    r.shift_left(v, count);
  }
};

template <typename Op, typename N, typename R, typename V1, typename V2>
struct numb_evaluator_rvv {
  inline void operator()(N& r, const V1& v1, const V2& v2) const {
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
  inline R operator()(numb<N>& q, const V1& v1, const V2& v2) const {
    return q.divrem(v1, v2);
  }
};

template <typename N, typename R, typename V1, typename V2>
struct divrem_evaluator<numb<N>, R, V1, V2> {
  inline void operator()(numb<N>& q, R& r, const V1& v1, const V2& v2) const {
    numb<N>::divrem(q, r, v1, v2);
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

template <typename N1, typename N2>
struct compare_eval<numb<N1>, numb<N2> > {
  inline int operator()(const numb<N1>& v1, const numb<N2>& v2) const {
    return detail::type_rank<numb<N1> >::value >= detail::type_rank<numb<N2> >::value
            ? v1.cmp(v2) : -v2.cmp(v1);
  }
};

template <typename N, typename V>
struct compare_eval<numb<N>, V> {
  inline int operator()(const numb<N>& v1, const V& v2) const {
    return v1.cmp(v2);
  }
};

template <typename V, typename N>
struct compare_eval<V, numb<N> > {
  inline int operator()(const V& v1, const numb<N>& v2) const {
    return -v2.cmp(v1);
  }
};

template <typename Op, typename N1, typename N2>
struct bool_compare_eval<Op, numb<N1>, numb<N2> > {
  inline int operator()(const numb<N1>& v1, const numb<N2>& v2) const {
    return bool_compare_eval<Op, int, int>()(compare(v1, v2), 0);
  }
};

template <typename Op, typename N, typename V>
struct bool_compare_eval<Op, numb<N>, V> {
  inline int operator()(const numb<N>& v1, const V& v2) const {
    return bool_compare_eval<Op, int, int>()(compare(v1, v2), 0);
  }
};

template <typename Op, typename V, typename N>
struct bool_compare_eval<Op, V, numb<N> > {
  inline int operator()(const V& v1, const numb<N>& v2) const {
    return bool_compare_eval<Op, int, int>()(compare(v1, v2), 0);
  }
};

template <typename T>
struct is_zero_eval<numb<T> > {
  inline bool operator()(const numb<T>& v) const {
    return v.is_zero();
  }
};

template <typename T>
struct is_positive_eval<numb<T> > {
  inline bool operator()(const numb<T>& v) const {
    return v.is_positive();
  }
};

template <typename T>
struct is_negative_eval<numb<T> > {
  inline bool operator()(const numb<T>& v) const {
    return v.is_negative();
  }
};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NUMBER_ABST_HPP
