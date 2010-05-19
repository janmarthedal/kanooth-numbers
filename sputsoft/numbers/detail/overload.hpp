/* 
 * File:   overload.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:21 PM
 */

#include <sputsoft/numbers/detail/expr.hpp>

#ifndef _OVERLOAD_HPP
#define	_OVERLOAD_HPP

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename R1, typename E1, typename R2, typename E2>
expr<typename resolve_binary<ops::binary::add, R1, R2>::return_type,
  binary<ops::binary::add, expr<R1, E1>, expr<R2, E2> > >
operator+(const expr<R1, E1>& x, const expr<R2, E2>& y)
{
  return expr<typename resolve_binary<ops::binary::add, R1, R2>::return_type,
          binary<ops::binary::add, expr<R1, E1>, expr<R2, E2> > >
            (binary<ops::binary::add, expr<R1, E1>, expr<R2, E2> >(x, y));
}

template <typename T>
T evaluate(const T& v)
{
  return v;
}

template <typename R>
const expr<R>& evaluate(const expr<R>& v)
{
  return v;
}

template <typename R1, typename R2, typename E2, typename R3, typename E3>
expr<R1> evaluate(const expr<R1, binary<ops::binary::add, expr<R2, E2>, expr<R3, E3> > >& v)
{
  expr<R1> r;
  sputsoft::numbers::add(r, evaluate(v.x), evaluate(v.y));
  return r;
}

template <typename R1, typename R2, typename R3, typename E3, typename R4, typename E4>
expr<R1>& operator=(expr<R1>& lhs, const expr<R2, binary<ops::binary::add, expr<R3, E3>, expr<R4, E4> > >& rhs)
{
  sputsoft::numbers::add(lhs, evaluate(rhs.x), evaluate(rhs.y));
  return lhs;
}


} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif	/* _OVERLOAD_HPP */
