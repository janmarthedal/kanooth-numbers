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

#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/detail/number_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T> class intnum;

} // namespace detail
} // namespace numbers

template <typename T>
struct make_unsigned<numbers::detail::numb<numbers::detail::intnum<T> > > {
  typedef T type;
};

template <typename T>
struct make_signed<numbers::detail::numb<numbers::detail::intnum<T> > > {
  typedef numbers::detail::numb<numbers::detail::intnum<T> > type;
};

template <typename T>
struct make_signed<numbers::detail::numb<numbers::detail::natnum<T> > > {
  typedef numbers::detail::numb<numbers::detail::intnum<numbers::detail::numb<numbers::detail::natnum<T> > > > type;
};

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

template <typename T, typename V>
struct evaluator_rv<ops::unary::bit_not, numb<intnum<T> >, V> {
  void operator()(numb<intnum<T> >& r, const V& v) const {
    sputsoft::numbers::negate(r, v);
    sputsoft::numbers::sub(r, r, 1);
  }
};

template <typename T>
struct function_v<ops::unary::bit_not, numb<intnum<T> > > {
  numb<intnum<T> > operator()(const numb<intnum<T> >& v) {
    numb<intnum<T> > r;
    evaluator_rv<ops::unary::bit_not, numb<intnum<T> >, numb<intnum<T> > >()(r, v);
    return r;
  }
};

template <typename T>
struct function_v<ops::unary::bit_not, numb<natnum<T> > > {
  numb<intnum<numb<natnum<T> > > > operator()(const numb<natnum<T> >& v) {
    numb<intnum<numb<natnum<T> > > > r;
    evaluator_rv<ops::unary::bit_not, numb<intnum<numb<natnum<T> > > >, numb<natnum<T> > >()(r, v);
    return r;
  }
};

/* bitwise_and */

template <typename N, typename V1, typename V2>
struct evaluator_rvv<ops::binary::bit_and, numb<intnum<N> >, V1, V2> {
  inline void operator()(numb<intnum<N> >& r, const V1& v1, const V2& v2) const {
    if (sputsoft::numbers::is_negative(v1)) {
      if (sputsoft::numbers::is_negative(v2)) {
        r.bitwise_or_pos(sputsoft::numbers::bitwise_not(v1), sputsoft::numbers::bitwise_not(v2));
        sputsoft::numbers::bitwise_not(r, r);
      } else
        r.bitwise_and_not_pos(v2, sputsoft::numbers::bitwise_not(v1));
    } else if (sputsoft::numbers::is_negative(v2))
      r.bitwise_and_not_pos(v1, sputsoft::numbers::bitwise_not(v2));
    else
      r.bitwise_and_pos(v1, v2);
  }
};

/* bitwise_or */

template <typename N, typename V1, typename V2>
struct evaluator_rvv<ops::binary::bit_or, numb<intnum<N> >, V1, V2> {
  inline void operator()(numb<intnum<N> >& r, const V1& v1, const V2& v2) const {
    if (sputsoft::numbers::is_negative(v1)) {
      if (sputsoft::numbers::is_negative(v2)) {
        r.bitwise_and_pos(sputsoft::numbers::bitwise_not(v1), sputsoft::numbers::bitwise_not(v2));
      } else
        r.bitwise_and_not_pos(sputsoft::numbers::bitwise_not(v1), v2);
      sputsoft::numbers::bitwise_not(r, r);
    } else if (sputsoft::numbers::is_negative(v2)) {
      r.bitwise_and_not_pos(sputsoft::numbers::bitwise_not(v2), v1);
      sputsoft::numbers::bitwise_not(r, r);
    } else
      r.bitwise_and_pos(v1, v2);
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
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP
