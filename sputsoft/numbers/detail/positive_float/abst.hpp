/*
 * File:   sputsoft/numbers/detail/positive_float/abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-08-24 08:57Z
 *
 * (C) Copyright SputSoft 2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_ABST_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_ABST_HPP

#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/number_traits.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename NUM, typename EXP, std::size_t DEFPREC=0> class posfloatnum;

} // namespace detail
} // namespace numbers

// Type traits related to natural numbers numb<posfloatnum<T, E, P> >

template <typename T, typename E, std::size_t P>
struct is_signed<numbers::detail::numb<numbers::detail::posfloatnum<T, E, P> > > : public false_type {};

template <typename T, typename E, std::size_t P>
struct make_unsigned<numbers::detail::numb<numbers::detail::posfloatnum<T, E, P> > > {
  typedef numbers::detail::numb<numbers::detail::posfloatnum<T, E, P> > type;
};

template <typename T, typename E, std::size_t P>
struct is_integral<numbers::detail::numb<numbers::detail::posfloatnum<T, E, P> > > : public false_type {};

namespace numbers {
namespace detail {

template <typename T, typename E, std::size_t P>
struct type_rank<numb<posfloatnum<T, E, P> > > : public integral_constant<int, 23> {};

template <typename T, typename E, std::size_t P>
struct unary_result2<ops::unary::trunc, numbers::detail::numb<numbers::detail::posfloatnum<T, E, P> > > {
  typedef T type;
};

template <typename T, typename E, std::size_t P>
struct evaluator_rv<ops::unary::trunc, T, numb<posfloatnum<T, E, P> > > {
  inline void operator()(T& r, const numb<posfloatnum<T, E, P> >& v) const {
    numb<posfloatnum<T, E, P> >::trunc(r, v);
  }
};

template <typename T, typename E, std::size_t P>
inline std::ostream& operator<<(std::ostream& os, const numb<posfloatnum<T, E, P> >& f) {
  T n;
  trunc(n, f);
  return os << n;
  //return os << f.get_num() << "(" << f.get_exponent() << ")";
}

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_ABST_HPP
