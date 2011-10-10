/*
 * File:   kanooth/numbers/detail/rational_number/rat_num_abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-10-05 10:51Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_ABST_HPP
#define	_KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_ABST_HPP

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/number_traits.hpp>

namespace kanooth {
namespace numbers {
namespace detail {

template <typename NUM, typename DEN> class ratnum;

} // namespace detail
} // namespace numbers

// Type traits related to rational numbers

template <typename N, typename D>
struct is_signed<numbers::detail::numb<numbers::detail::ratnum<N, D> > >
        : public true_type {};

template <typename N, typename D>
struct make_signed<numbers::detail::numb<numbers::detail::ratnum<N, D> > >
        : public set_type<numbers::detail::numb<numbers::detail::ratnum<N, D> > > {};

template <typename N, typename D>
struct is_integral<numbers::detail::numb<numbers::detail::ratnum<N, D> > >
        : public false_type {};

namespace numbers {
namespace detail {

template <typename N, typename D>
struct type_rank<numb<ratnum<N, D> > > : public integral_constant<int, 10> {};

template <typename N, typename D>
struct unary_result2<ops::unary::trunc, numb<ratnum<N, D> > >
        : public set_type<N> {};

template <typename N, typename D>
struct function_v<ops::unary::trunc, numb<ratnum<N, D> > >
        : public function_v_fallback<ops::unary::trunc, numb<ratnum<N, D> > > {};

template <typename N, typename D>
struct evaluator_rv<ops::unary::floor, N, numb<ratnum<N, D> > > {
  inline void operator()(N& r, const numb<ratnum<N, D> >& v) const {
    numb<ratnum<N, D> >::floor(r, v);
  }
};

template <typename N, typename D, typename V1, typename V2>
struct evaluator_rvv<ops::binary::div, numb<ratnum<N, D> >, V1, V2> {
  inline void operator()(numb<ratnum<N, D> >& r, const V1& v1, const V2& v2) const {
    r.div(v1, v2);
  }
};
    
/*
template <typename N, typename D>
struct evaluator_rv<ops::unary::trunc, T, numb<posfloatnum<T, E, P> > >
        : public evaluator_rv<ops::unary::floor, T, numb<posfloatnum<T, E, P> > > {};

template <typename N, typename D>
template <typename T, typename E, std::size_t P>
struct evaluator_rv<ops::unary::round, T, numb<posfloatnum<T, E, P> > > {
  inline void operator()(T& r, const numb<posfloatnum<T, E, P> >& v) const {
    numb<posfloatnum<T, E, P> >::round(r, v);
  }
};

*/

template <typename N, typename D>
inline std::ostream& operator<<(std::ostream& os, const numb<ratnum<N, D> >& n) {
  return os << n.get_numerator() << "/" << n.get_denominator();
}

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_ABST_HPP
