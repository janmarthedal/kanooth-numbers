/* 
 * File:   sputsoft/numbers/detail/named_ops.hpp
 * Author: jmr
 *
 * Created 2010-04-30 15:04Z
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP

#include <cstring>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R, typename V>
struct set_2_eval {
  inline void set(R& r, const V& v) { r = v; }
};
template <typename R, typename V>
struct negate_2_eval {
  inline void negate(R& r, const V& v) { r = -v; }
};
template <typename V1, typename V2>
struct cmp_r2_eval {
  static int cmp(const V1& v1, const V2& v2) {
    return v1 < v2 ? -1 : v1 > v2 ? 1 : 0;
  }
};
template <typename V1, typename V2>
struct equal_r2_eval {
  static bool equal(const V1& v1, const V2& v2) {
    return cmp_r2_eval<V1, V2>::cmp(v1, v2) == 0;
  }
};
template <typename T>
struct log2_floor_evaluator {
  static std::size_t log2_floor(T n) {
    std::size_t r = -1;
    while (n) { ++r; n >>= 1; }
    return r;
  }
};
template <typename R, typename Forw>                        struct set_4_eval;
template <typename R, typename V1, typename V2>             struct add_3_eval;
template <typename R, typename V1, typename V2>             struct sub_3_eval;
template <typename R, typename V1, typename V2>             struct mul_3_eval;
template <typename R, typename V1, typename V2>             struct div_3_eval;
template <typename R, typename V1, typename V2>             struct div_floor_3_eval;
template <typename R, typename V1, typename V2>             struct div_ceil_3_eval;
template <typename R, typename V1, typename V2>             struct div_trunc_3_eval;
template <typename R, typename V1, typename V2>             struct rem_3_eval;
template <typename R, typename V1, typename V2>             struct rem_floor_3_eval;
template <typename R, typename V1, typename V2>             struct rem_ceil_3_eval;
template <typename R, typename V1, typename V2>             struct rem_trunc_3_eval;
template <typename V1, typename V2>                         struct rem_r2_eval;
template <typename V1, typename V2>                         struct rem_floor_r2_eval;
template <typename V1, typename V2>                         struct rem_ceil_r2_eval;
template <typename V1, typename V2>                         struct rem_trunc_r2_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_floor_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_ceil_4_eval;
template <typename Q, typename R, typename V1, typename V2> struct quotrem_trunc_4_eval;
template <typename Q, typename V1, typename V2>             struct quotrem_r3_eval;
template <typename Q, typename V1, typename V2>             struct quotrem_floor_r3_eval;
template <typename Q, typename V1, typename V2>             struct quotrem_ceil_r3_eval;
template <typename Q, typename V1, typename V2>             struct quotrem_trunc_r3_eval;

} // namespace detail

template <typename R, typename V>
inline void set(R& r, const V& v) {
  detail::set_2_eval<R, V>::set(r, v);
}

template <typename R, typename V>
inline void negate(R& r, const V& v) {
  detail::negate_2_eval<R, V>::negate(r, v);
}

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

template <typename R, typename V1, typename V2>
inline void add(R& r, const V1& v1, const V2& v2) {
  detail::add_3_eval<R, V1, V2>::add(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void sub(R& r, const V1& v1, const V2& v2) {
  detail::sub_3_eval<R, V1, V2>::sub(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void mul(R& r, const V1& v1, const V2& v2) {
  detail::mul_3_eval<R, V1, V2>::mul(r, v1, v2);
}

template <typename R, typename V1, typename V2>
inline void div(R& r, const V1& v1, const V2& v2) {
  detail::div_3_eval<R, V1, V2>::div(r, v1, v2);
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
  detail::rem_3_eval<R, V1, V2>::rem(r, v1, v2);
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

template <typename V1, typename V2>
inline V2 rem(const V1& v1, const V2& v2) {
  return detail::rem_r2_eval<V1, V2>::rem(v1, v2);
}

template <typename V1, typename V2>
inline V2 rem_floor(const V1& v1, const V2& v2) {
  return detail::rem_floor_r2_eval<V1, V2>::rem_floor(v1, v2);
}

template <typename V1, typename V2>
inline V2 rem_ceil(const V1& v1, const V2& v2) {
  return detail::rem_ceil_r2_eval<V1, V2>::rem_ceil(v1, v2);
}

template <typename V1, typename V2>
inline V2 rem_trunc(const V1& v1, const V2& v2) {
  return detail::rem_trunc_r2_eval<V1, V2>::rem_trunc(v1, v2);
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

template <typename Q, typename V1, typename V2>
inline V2 quotrem(Q& q, const V1& v1, const V2& v2) {
  return detail::quotrem_r3_eval<Q, V1, V2>::quotrem(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline V2 quotrem_ceil(Q& q, const V1& v1, const V2& v2) {
  return detail::quotrem_ceil_r3_eval<Q, V1, V2>::quotrem_ceil(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline V2 quotrem_floor(Q& q, const V1& v1, const V2& v2) {
  return detail::quotrem_floor_r3_eval<Q, V1, V2>::quotrem_floor(q, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline V2 quotrem_trunc(Q& q, const V1& v1, const V2& v2) {
  return detail::quotrem_trunc_r3_eval<Q, V1, V2>::quotrem_trunc(q, v1, v2);
}

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

} // namespace numbers
} // namespace sputsoft

#endif	/* _SPUTSOFT_NUMBERS_DETAIL_NAMED_OPS_HPP */
