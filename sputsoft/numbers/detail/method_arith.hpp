/* 
 * File:   method_arith.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:04 PM
 *
 * $Id$
 */

#ifndef _METHOD_ARITH_HPP
#define	_METHOD_ARITH_HPP

#include <cstring>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R, typename V>
struct set_2_eval;
template <typename R, typename Forw>
struct set_4_eval;
template <typename R, typename V1, typename V2>
struct add_3_eval;
template <typename R, typename V1, typename V2>
struct sub_3_eval;
template <typename R, typename V1, typename V2>
struct mul_3_eval;
template <typename R, typename V1, typename V2>
struct div_3_eval;
template <typename R, typename V1, typename V2>
struct rem_3_eval;
template <typename V1, typename V2>
struct rem_r2_eval;
template <typename Q, typename R, typename V1, typename V2>
struct quotrem_4_eval;
template <typename Q, typename V1, typename V2>
struct quotrem_r3_eval;
template <typename T>
struct floor_log2_evaluator {
  static std::size_t floor_log2(T n) {
    std::size_t r = -1;
    while (n) { ++r; n >>= 1; }
    return r;
  }
};
template <typename V1, typename V2>
struct cmp_r2_eval {
  static int cmp(const V1& v1, const V2& v2) {
    return v1 < v2 ? -1 : v1 > v2 ? 1 : 0;
  }
};

} // namespace sputsoft

template <typename R, typename V>
inline void set(R& r, const V& v) {
  detail::set_2_eval<R, V>::set(r, v);
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
inline void rem(R& r, const V1& v1, const V2& v2) {
  detail::rem_3_eval<R, V1, V2>::rem(r, v1, v2);
}

template <typename V1, typename V2>
inline V2 rem(const V1& v1, const V2& v2) {
  return detail::rem_r2_eval<V1, V2>::rem(v1, v2);
}

template <typename Q, typename R, typename V1, typename V2>
inline void quotrem(Q& q, R& r, const V1& v1, const V2& v2) {
  detail::quotrem_4_eval<Q, R, V1, V2>::quotrem(q, r, v1, v2);
}

template <typename Q, typename V1, typename V2>
inline V2 quotrem(Q& q, const V1& v1, const V2& v2) {
  return detail::quotrem_r3_eval<Q, V1, V2>::quotrem(q, v1, v2);
}

template <typename T>
inline std::size_t floor_log2(T n) {
  return detail::floor_log2_evaluator<T>::floor_log2(n);
}

template <typename V1, typename V2>
inline int compare(const V1& v1, const V2& v2) {
  return detail::cmp_r2_eval<V1, V2>::cmp(v1, v2);
}

} // namespace detail
} // namespace numbers

#endif	/* _METHOD_ARITH_HPP */

