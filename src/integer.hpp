/*
 * File:   integer.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created on 5. maj 2009, 15:52
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _INTEGER_HPP
#define	_INTEGER_HPP

#include <iostream>
#include <natural_number.hpp>

namespace sputsoft {
namespace multiprecision {


template <typename N>
class integer {
private:
  bool positive;
  N number;
public:
  integer(const N& n, const bool pos=true) : positive(pos), number(n) {}
  integer operator-() const { return integer(number, !positive); }
  bool isZero() const { return number.isZero(); }
  bool isPositive() const { return !isZero() && positive; }
  bool isNegative() const { return !isZero() && !positive; }
  static N abs(const integer<N>& u);
  static integer add(const integer<N>& x, const integer<N>& y);
  static integer subtract(const integer<N>& x, const integer<N>& y);
  static integer multiply(const integer<N>& x, const integer<N>& y);
};

template <typename N>
N integer<N>::abs(const integer<N>& u)
{
  return u.number;
}

template <typename N>
integer<N> integer<N>::add(const integer<N>& x, const integer<N>& y)
{
  if (x.isZero()) return y;
  if (y.isZero()) return x;
  if (x.isPositive() == y.isPositive())
    return integer<N>(x.number + y.number, x.isPositive());
  bool xLarger = x.number > y.number;
  return integer<N>(xLarger ? x.number - y.number : y.number - x.number, xLarger == x.isPositive());
}

template <typename N>
integer<N> integer<N>::subtract(const integer<N>& x, const integer<N>& y)
{
  if (x.isZero()) return -y;
  if (y.isZero()) return x;
  if (x.isPositive() != y.isPositive())
    return integer<N>(x.number + y.number, x.isPositive());
  bool xLarger = x.number > y.number;
  return integer<N>(xLarger ? x.number - y.number : y.number - x.number, xLarger == x.isPositive());
}

template <typename N>
integer<N> integer<N>::multiply(const integer<N>& x, const integer<N>& y)
{
  return integer<N>(x.number * y.number, x.positive == y.positive);
}

template <typename N>
inline integer<N> operator*(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::multiply(u, v);
}

template <typename N>
std::ostream& operator<<(std::ostream& os, const integer<N>& n)
{
  if (n.isNegative()) os << "-";
  os << abs(n);
  return os;
}


} // multiprecision
} // sputsoft


#endif	/* _INTEGER_HPP */

