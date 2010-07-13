/* 
 * File:   sputsoft/numbers/detail/int_abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-13 11:00Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP

#include <sputsoft/detail/types.hpp>
#include <sputsoft/numbers/detail/number_abst.hpp>

#include "int_impl.hpp"

namespace sputsoft {
namespace numbers {
namespace detail {

/* Return types */

#define RESOLVE_INTNUM_REM(TYPE) \
template <typename T> \
struct resolve_binary<ops::binary::rem, numb<intnum<T> >, TYPE> { typedef TYPE return_type; }; \
template <typename T> \
struct resolve_binary<ops::binary::rem_floor, numb<intnum<T> >, TYPE> { typedef TYPE return_type; }; \
template <typename T> \
struct resolve_binary<ops::binary::rem_ceil, numb<intnum<T> >, TYPE> { typedef TYPE return_type; }; \
template <typename T> \
struct resolve_binary<ops::binary::rem_trunc, numb<intnum<T> >, TYPE> { typedef TYPE return_type; };

RESOLVE_INTNUM_REM(unsigned short)
RESOLVE_INTNUM_REM(signed short)
RESOLVE_INTNUM_REM(unsigned)
RESOLVE_INTNUM_REM(signed)
RESOLVE_INTNUM_REM(unsigned long)
RESOLVE_INTNUM_REM(signed long)
#ifdef SPUTSOFT_HAS_LONG_LONG
RESOLVE_INTNUM_REM(unsigned long long)
RESOLVE_INTNUM_REM(signed long long)
#endif

template <typename T>
struct resolve_unary<ops::unary::abs, numb<intnum<T> > > {
  typedef T return_type;
};

/***************/

template <typename T, typename V1, typename V2>
struct evaluator_rvv<ops::binary::div, numb<intnum<T> >, V1, V2> {
  void operator()(numb<intnum<T> >& r, const V1& v1, const V2& v2) const {
    r.div_floor(v1, v2);
  }
};

template <typename R, typename T, typename V>
struct evaluator_rvv<ops::binary::rem, R, numb<intnum<T> >, V> {
  void operator()(R& r, const numb<intnum<T> >& v1, const V& v2) const {
    numb<intnum<T> >::rem_floor(r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct evaluator_rrvv<ops::binary::quotrem, numb<intnum<T> >, R, V1, V2> {
  void operator()(numb<intnum<T> >& q, R& r, const V1& v1, const V2& v2) const {
    numb<intnum<T> >::quotrem_floor(q, r, v1, v2);
  }
};

/* Unary negate */

template <typename N>
struct evaluator_rv<ops::unary::negate, numb<intnum<N> >, numb<intnum<N> > > {
  void operator()(numb<intnum<N> >& r, const numb<intnum<N> >& v) const{
    r.negate(v);
  }
};

template <typename N>
struct evaluator_rv<ops::unary::negate, numb<intnum<N> >, N> {
  void operator()(numb<intnum<N> >& r, const N& v) const{
    r.negate(v);
  }
};

/* Unary abs */

template <typename T>
struct evaluator_rv<ops::unary::abs, T, numb<intnum<T> > > {
  void operator()(T& r, const numb<intnum<T> >& v) const {
    v.set_abs(r);
  }
};

/* bitwise_not */

/*template <typename T, typename V>
struct not_2_eval<numb<intnum<T> >, V> {
  static void bit_not(numb<intnum<T> >& r, const V& v) {
    sputsoft::numbers::negate(r, v);
    sputsoft::numbers::sub(r, r, 1);
  }
};*/

/* bitwise_and */

template <typename V>
typename resolve_unary<ops::unary::abs, V>::return_type bit_abs(const V& v) {
  typename resolve_unary<ops::unary::abs, V>::return_type n = sputsoft::numbers::abs(v);
  if (sputsoft::numbers::is_negative(v))
    sputsoft::numbers::sub(n, n, 1);
  return n;
}

template <typename N, typename V1, typename V2>
void bitwise_and_help(numb<intnum<N> >& r, const V1& v1, const V2& v2) {
  typename resolve_unary<ops::unary::abs, V1>::return_type n1 = bit_abs(v1);
  typename resolve_unary<ops::unary::abs, V2>::return_type n2 = bit_abs(v2);
  if (sputsoft::numbers::is_negative(v1)) {
    if (sputsoft::numbers::is_negative(v2)) {
      sputsoft::numbers::bitwise_or(n1, n1, n2);
      sputsoft::numbers::bitwise_not(r, n1);
    } else {
      sputsoft::numbers::bitwise_and_not(n2, n2, n1);
      sputsoft::numbers::set(r, n2);
    }
  } else if (sputsoft::numbers::is_negative(v2)) {
    sputsoft::numbers::bitwise_and_not(n1, n1, n2);
    sputsoft::numbers::set(r, n1);
  } else
    sputsoft::numbers::bitwise_and(r, n1, n2);
}

template <typename N>
struct evaluator_rvv<ops::binary::bit_and, numb<intnum<N> >, numb<intnum<N> >, numb<intnum<N> > > {
  inline void operator()(numb<intnum<N> >& r, const numb<intnum<N> >& v1, const numb<intnum<N> >& v2) const {
    bitwise_and_help(r, v1, v2);
  }
};

template <typename N>
struct evaluator_rvv<ops::binary::bit_and, numb<intnum<N> >, int, numb<intnum<N> > > {
  inline void operator()(numb<intnum<N> >& r, int v1, const numb<intnum<N> >& v2) const {
    bitwise_and_help(r, v1, v2);
  }
};

template <typename N>
struct evaluator_rvv<ops::binary::bit_and, numb<intnum<N> >, numb<intnum<N> >, int> {
  inline void operator()(numb<intnum<N> >& r, const numb<intnum<N> >& v1, int v2) const {
    bitwise_and_help(r, v1, v2);
  }
};

/* bitwise_or */

/*template <typename N, typename V1, typename V2>
struct or_3_eval<numb<intnum<N> >, V1, V2> {
  static void bit_or(numb<intnum<N> >& r, const V1& v1, const V2& v2) {
    typename resolve_unary<ops::unary::abs, V1>::return_type n1 = bit_abs(v1);
    typename resolve_unary<ops::unary::abs, V2>::return_type n2 = bit_abs(v2);
    if (sputsoft::numbers::is_negative(v1)) {
      if (sputsoft::numbers::is_negative(v2))
        sputsoft::numbers::bitwise_and(n1, n1, n2);
      else
        sputsoft::numbers::bitwise_and_not(n1, n1, n2);
      sputsoft::numbers::bitwise_not(r, n1);
    } else if (sputsoft::numbers::is_negative(v2)) {
      sputsoft::numbers::bitwise_and_not(n1, n1, n2);
      sputsoft::numbers::set(r, n1);
    } else {
      sputsoft::numbers::bitwise_or(n1, n1, n2);
      sputsoft::numbers::set(r, n1);
    }
  }
};*/

template <typename T>
struct is_positive_r1_eval<numb<intnum<T> > > {
  static inline bool is_positive(const numb<intnum<T> >& v) {
    return v.is_positive();
  }
};

template <typename T>
struct is_negative_r1_eval<numb<intnum<T> > > {
  static inline bool is_negative(const numb<intnum<T> >& v) {
    return v.is_negative();
  }
};

template <typename NUM>
inline std::ostream& operator<<(std::ostream& os, const numb<intnum<NUM> >& n) {
  if (n.num && !n.positive) os << "-";
  return os << n.num;
}

template <typename NUM, typename Forw>
struct set_4_eval<numb<intnum<NUM> >, Forw> {
  static void set(numb<intnum<NUM> >& n, Forw first, const Forw last, unsigned base) {
    bool pos=true;
    while (first != last) {
      char c = *first;
      if (c == '-') pos = !pos;
      else if (c != '+') break;
      ++first;
    }
    NUM t;
    sputsoft::numbers::set(t, first, last, base);
    n = numb<intnum<NUM> >(t, pos);
  }
};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP
