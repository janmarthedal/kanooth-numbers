/* 
 * File:   sputsoft/numbers/detail/integer/abst.hpp
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
#include <sputsoft/numbers/number_traits.hpp>
#include <sputsoft/numbers/detail/natural_number/abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T> class intnum;

} // namespace detail
} // namespace numbers

template <typename T>
struct is_signed<numbers::detail::numb<numbers::detail::intnum<T> > > : public true_type {};

template <typename T>
struct is_integral<numbers::detail::numb<numbers::detail::intnum<T> > > : public true_type {};

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

template <typename T>
struct type_rank<numb<intnum<T> > > : public integral_constant<int, 21> {};

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

template <typename N>
struct evaluator_rv<ops::unary::abs, numb<intnum<N> >, numb<intnum<N> > > {
  void operator()(numb<intnum<N> >& r, const numb<intnum<N> >& v) const{
    r.abs(v);
  }
};

template <typename N>
struct evaluator_rv<ops::unary::abs, N, numb<intnum<N> > > {
  void operator()(N& r, const numb<intnum<N> >& v) const{
    numb<intnum<N> >::abs(r, v);
  }
};

template <typename Op, typename T>
struct function_v_fallback {
  typedef typename unary_result<Op, T>::type return_type;
  inline const return_type operator()(const T& v) const {
    return_type r;
    evaluator_rv<Op, return_type, T>()(r, v);
    return r;
  }
};


template <typename T>
struct function_v<ops::unary::abs, numb<intnum<T> > >
        : public function_v_fallback<ops::unary::abs, numb<intnum<T> > > {};

/* bitwise_not */

template <typename T, typename V>
struct evaluator_rv<ops::unary::bit_not, numb<intnum<T> >, V> {
  void operator()(numb<intnum<T> >& r, const V& v) const {
    sputsoft::numbers::negate(r, v);
    sputsoft::numbers::sub(r, r, 1);
  }
};

template <typename T>
struct function_v<ops::unary::bit_not, numb<intnum<T> > >
        : public function_v_fallback<ops::unary::bit_not, numb<intnum<T> > > {};

template <typename T>
struct function_v<ops::unary::bit_not, numb<natnum<T> > >
        : public function_v_fallback<ops::unary::bit_not, numb<natnum<T> > > {};

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
struct is_positive_eval<numb<intnum<T> > > {
  inline bool operator()(const numb<intnum<T> >& v) {
    return v.is_positive();
  }
};

template <typename T>
struct is_negative_eval<numb<intnum<T> > > {
  inline bool operator()(const numb<intnum<T> >& v) {
    return v.is_negative();
  }
};

template <typename NUM>
inline std::ostream& operator<<(std::ostream& os, const numb<intnum<NUM> >& i) {
  if (i.is_negative()) os << "-";
  return os << abs(i);
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
