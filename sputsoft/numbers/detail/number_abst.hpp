/* 
 * File:   number_abst.hpp
 * Author: jmr
 *
 * Created 2010-05-19, 15:10Z
 *
 * $Id$
 */

#ifndef _NUMBER_ABST_HPP
#define	_NUMBER_ABST_HPP

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
struct resolve_binary<Op, numb<T>, unsigned> {
  typedef numb<T> return_type;
};
template <typename Op, typename T>
struct resolve_binary<Op, unsigned, numb<T> > {
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

#ifdef SPUTSOFT_HAS_LONG_LONG
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

template <typename T, typename V1, typename V2>
struct rem_floor_3_eval<numb<T>, V1, V2> {
  static void rem_floor(numb<T>& r, const V1& v1, const V2& v2) {
    r.rem_floor(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct rem_ceil_3_eval<numb<T>, V1, V2> {
  static void rem_ceil(numb<T>& r, const V1& v1, const V2& v2) {
    r.rem_ceil(v1, v2);
  }
};

template <typename T, typename V1, typename V2>
struct rem_trunc_3_eval<numb<T>, V1, V2> {
  static void rem_trunc(numb<T>& r, const V1& v1, const V2& v2) {
    r.rem_trunc(v1, v2);
  }
};

template <typename Q, typename R, typename T1, typename V2>
struct quotrem_4_eval<Q, R, numb<T1>, V2> {
  static void quotrem(Q& q, R& r, const numb<T1>& v1, const V2& v2) {
    numb<T1>::quotrem(q, r, v1, v2);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _NUMBER_ABST_HPP */

