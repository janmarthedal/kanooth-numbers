/*
 * File:   kanooth/numbers/detail/floating_point/float_abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-10-12 11:48Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_FLOAT_ABST_HPP
#define	_KANOOTH_NUMBERS_DETAIL_FLOAT_ABST_HPP

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/number_traits.hpp>
#include <kanooth/numbers/detail/positive_float/pos_float_abst.hpp>

namespace kanooth {
namespace numbers {
namespace detail {

template <typename T> class floatnum;

} // namespace detail
} // namespace numbers

template <typename T>
struct is_signed<numbers::detail::numb<numbers::detail::floatnum<T> > >
  : public true_type {};

template <typename T>
struct is_integral<numbers::detail::numb<numbers::detail::floatnum<T> > >
  : public false_type {};

template <typename T>
struct make_unsigned<numbers::detail::numb<numbers::detail::floatnum<T> > >
  : public set_type<T> {};

template <typename T>
struct make_signed<numbers::detail::numb<numbers::detail::floatnum<T> > >
  : public set_type<numbers::detail::numb<numbers::detail::floatnum<T> > > {};

template <typename N, typename E, std::size_t D>
struct make_signed<numbers::detail::numb<numbers::detail::posfloatnum<N, E, D> > >
  : public set_type<numbers::detail::numb<numbers::detail::floatnum<numbers::detail::numb<numbers::detail::posfloatnum<N, E, D> > > > > {};

namespace numbers {
namespace detail {

template <typename T>
struct type_rank<numb<floatnum<T> > > : public integral_constant<int, 24> {};

/* Unary negate */

template <typename N>
struct evaluator_rv<ops::unary::negate, numb<floatnum<N> >, numb<floatnum<N> > > {
  void operator()(numb<floatnum<N> >& r, const numb<floatnum<N> >& v) const{
    r.negate(v);
  }
};

template <typename N>
struct evaluator_rv<ops::unary::negate, numb<floatnum<N> >, N> {
  void operator()(numb<floatnum<N> >& r, const N& v) const{
    r.negate(v);
  }
};

/* Unary abs */

template <typename N>
struct evaluator_rv<ops::unary::abs, numb<floatnum<N> >, numb<floatnum<N> > > {
  void operator()(numb<floatnum<N> >& r, const numb<floatnum<N> >& v) const{
    r.abs(v);
  }
};

template <typename N>
struct evaluator_rv<ops::unary::abs, N, numb<floatnum<N> > > {
  void operator()(N& r, const numb<floatnum<N> >& v) const{
    numb<floatnum<N> >::abs(r, v);
  }
};


template <typename T>
struct function_v<ops::unary::abs, numb<floatnum<T> > >
        : public function_v_fallback<ops::unary::abs, numb<floatnum<T> > > {};


template <typename T>
struct is_positive_eval<numb<floatnum<T> > > {
  inline bool operator()(const numb<floatnum<T> >& v) {
    return v.is_positive();
  }
};

template <typename T>
struct is_negative_eval<numb<floatnum<T> > > {
  inline bool operator()(const numb<floatnum<T> >& v) {
    return v.is_negative();
  }
};

template <typename NUM>
inline std::ostream& operator<<(std::ostream& os, const numb<floatnum<NUM> >& n) {
  if (n.is_negative()) os << "-";
  return os << abs(n);
}

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_FLOAT_ABST_HPP
