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

#include <sputsoft/numbers/detail/number_abst.hpp>

#include "int_impl.hpp"

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T> class intnum;

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


template <typename T, typename V1, typename V2>
struct div_3_eval<numb<intnum<T> >, V1, V2> {
  static void div(numb<intnum<T> >& r, const V1& v1, const V2& v2) {
    r.div_floor(v1, v2);
  }
};

template <typename R, typename T1, typename V2>
struct rem_3_eval<R, numb<intnum<T1> >, V2> {
  static void rem(R& r, const numb<intnum<T1> >& v1, const V2& v2) {
    numb<intnum<T1> >::rem_floor(r, v1, v2);
  }
};

template <typename T, typename R, typename V1, typename V2>
struct quotrem_4_eval<numb<intnum<T> >, R, V1, V2> {
  static void quotrem(numb<intnum<T> >& q, R& r, const V1& v1, const V2& v2) {
    numb<intnum<T> >::quotrem_floor(q, r, v1, v2);
  }
};

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
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP
